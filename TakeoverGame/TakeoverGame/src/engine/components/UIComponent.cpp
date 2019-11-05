#include "UIComponent.h"
#include "../core/Window.h"
#include "../components/CameraComponent.h"

using namespace vlk;

namespace
{
	Vector2 topRight;
	Vector2 bottomLeft;

	void OnFrameBuffer(WindowFramebufferEvent& ev)
	{
		topRight = Vector2(ev.width / 2.0f, ev.height / 2.0f);
		bottomLeft = Vector2(-ev.width / 2.0f, -ev.height / 2.0f);
	}

	void OnUpdate(UpdateEvent& ev)
	{
		auto update = UIComponent::ForEach([](UIComponent* c)
		{
			switch (c->dock)
			{
				case DockType::TopLeft:
					c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y + topRight.y);
					break;
				case DockType::TopCenter:
					c->transform->location = Vector2(c->offset.x, c->offset.y + topRight.y);
					break;
				case DockType::TopRight:
					c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y + topRight.y);
					break;
				case DockType::CenterLeft:
					c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y);
					break;
				case DockType::Center:
					c->transform->location = Vector2(c->offset.x, c->offset.y);
					break;
				case DockType::CenterRight:
					c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y);
					break;
				case DockType::BottomLeft:
					c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y + bottomLeft.y);
					break;
				case DockType::BottomCenter:
					c->transform->location = Vector2(c->offset.x, c->offset.y + bottomLeft.y);
					break;
				case DockType::BottomRight:
					c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y + bottomLeft.y);
					break;
				default:
					break;
			}
		});
	}
}

void UISystem::Init()
{
	EventBus<WindowFramebufferEvent>::Get().AddEventListener(OnFrameBuffer);
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void UISystem::Destroy()
{
	EventBus<WindowFramebufferEvent>::Get().RemoveEventListener(OnFrameBuffer);
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

UIComponent::UIComponent(IEntity* e, TransformComponent2D* transform) :
	Component(e)
{
	this->transform = transform;
	this->dock = DockType::Center;
}