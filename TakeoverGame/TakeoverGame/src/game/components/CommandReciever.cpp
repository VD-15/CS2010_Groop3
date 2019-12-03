#include "CommandReciever.h"

#include "../../engine/core/VLKTime.h"

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
					c->target = ev.target;
					c->isMoving = true;
				}
			}
		});
	}
	-
	void OnUpdate(UpdateEvent& ev)
	{
		CommandRecieverComponent::ForEach([](CommandRecieverComponent* c)
		{
			if (c->isMoving)
			{
				Vector2 moveDir = c->target - c->transform->location;

				Vector2 moveAmount(Vector2::Normalize(moveDir) * c->speed * VLKTime::DeltaTime<Float>());

				if (moveAmount.Magnitude() > moveDir.Magnitude())
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
}

void CommandRecieverSystem::Destroy()
{
	EventBus<CommandIssuedEvent>::Get().RemoveEventListener(OnCommandRecieved);
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

CommandIssuedEvent::CommandIssuedEvent(Vector2 target) :
	target(target)
{

}

CommandRecieverComponent::CommandRecieverComponent(IEntity* e, TransformComponent2D* transform, const SelectableComponent* const select) :
	Component<CommandRecieverComponent>(e),
	transform(transform),
	select(select)
{
	this->isMoving = false;
	this->speed = 0.0f;
}