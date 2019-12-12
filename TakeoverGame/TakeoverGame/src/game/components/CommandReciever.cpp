#include "CommandReciever.h"

#include "../../engine/core/VLKTime.h"
#include "../../engine/components/CameraComponent.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/Window.h"

using namespace tkv;

namespace
{
	void OnCommandRecieved(CommandIssuedEvent& ev)
	{
		CommandRecieverComponent::ForEach([&ev](CommandRecieverComponent* c)
		{
			if (c->select->flags & TKV_FLAG_SELECTED)
			{
				if (c->target != ev.target)
				{
					LogInfo("CommandReciever", "Command recieved!");
					c->target = ev.target;
					c->isMoving = true;
				}
			}
		});
	}

	//Diapatch commands here
	void OnPreUpdate(PreUpdateEvent& ev)
	{
		if (Mouse::IsButtonPressed(MouseButtons::BUTTON_RIGHT))
		{
			//Get mouse position relative to center of screen
			Vector2 mousePos(Mouse::GetCenteredPosition());
			mousePos = Vector2(mousePos.x / (Window::GetWidth() / 2.0f), mousePos.y / (Window::GetHeight() / 2.0f));

			//Inverted projection matrix to fund world coords from screen space
			Matrix4 inv(glm::inverse(CameraComponent3D::ACTIVE->GetProjection()));// * CreateRotation(c->camera->transform->rotation)));

			Vector4 v(mousePos, 0.0f, 1.0f);

			//Direction of mouse cursor from camera in world space
			Vector3 dir(inv * v);
			dir = Rotate(dir, CameraComponent3D::ACTIVE->transform->rotation);

			Ray r(dir, CameraComponent3D::ACTIVE->transform->location);

			CircleVolume volume;
			volume.radius = 1024.0f;

			if (volume.Intersects(r))
			{
				Vector3 newTarget = volume.GetIntersection(r);

				LogInfo("CommandReciever", "Command dispatched, Target: " + 
						std::to_string(newTarget.x) + ", " + 
						std::to_string(newTarget.y) + ", " + 
						std::to_string(newTarget.z)
				);
				EventBus<CommandIssuedEvent>::Get().PostEvent({ newTarget });
			}
		}
	}

	void OnUpdate(UpdateEvent& ev)
	{
		CommandRecieverComponent::ForEach([](CommandRecieverComponent* c)
		{
			if (c->isMoving)
			{
				Vector3 moveDir = c->target - c->transform->location;

				Vector3 moveAmount(Normalize(moveDir) * c->speed * VLKTime::DeltaTime<Float>());

				if (Magnitude(moveAmount) > Magnitude(moveDir))
				{
					c->transform->location = c->target;
					c->isMoving = false;
				}
				else
				{
					c->transform->location += moveAmount;
				}
			}
		});
	}
}

void CommandRecieverSystem::Init()
{
	EventBus<CommandIssuedEvent>::Get().AddEventListener(OnCommandRecieved);
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
	EventBus<PreUpdateEvent>::Get().AddEventListener(OnPreUpdate);
}

void CommandRecieverSystem::Destroy()
{
	EventBus<CommandIssuedEvent>::Get().RemoveEventListener(OnCommandRecieved);
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
	EventBus<PreUpdateEvent>::Get().RemoveEventListener(OnPreUpdate);
}

CommandIssuedEvent::CommandIssuedEvent(Vector3 target) :
	target(target)
{

}

CommandRecieverComponent::CommandRecieverComponent(IEntity* e, TransformComponent3D* transform, const SelectableComponent* const select) :
	Component<CommandRecieverComponent>(e),
	transform(transform),
	select(select),
	target()
{
	this->isMoving = false;
	this->speed = 0.0f;
}