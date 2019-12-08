#include "Selectable.h"
#include "../ui/Cursor.h"
#include "../../engine/components/CameraComponent.h"
#include "../../engine/core/ContentManager.hpp"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/Keyboard.h"
#include "../../engine/core/Window.h"
#include "../../engine/core/VLKTime.h"

using namespace tkv;

namespace
{
	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		const CameraComponent3D* camera = CameraComponent3D::ACTIVE;
		Vector2 mousePos(Mouse::GetCenteredPosition());
		mousePos = Vector2(mousePos.x / (Window::GetWidth() / 2.0f), mousePos.y / (Window::GetHeight() / 2.0f));

		Matrix4 inv(glm::inverse(camera->GetProjection()));// * CreateRotation(c->camera->transform->rotation)));

		Vector4 v(mousePos, 0.0f, 1.0f);

		Vector3 dir(inv * v);
		dir = Rotate(dir, camera->transform->rotation);

		Ray r(dir, camera->transform->location);
		Bool select = Mouse::IsButtonPressed(MouseButtons::BUTTON_LEFT);
		Bool multi = Keyboard::IsKeyDown(Keys::LEFT_SHIFT);
		Float dt = VLKTime::DeltaTime<Float>();

		SelectableComponent::ForEach([&r, select, multi, dt](SelectableComponent* c)
		{
			c->transform->rotation *= AngleAxis(dt, Vector3Y);

			Vector3 circlePos(c->transform->location);
			circlePos.y = 0.0f;
			CircleVolume volume(circlePos, c->hoverRadius);

			if (volume.Intersects(r)) //If the ray is intersecting
			{
				if (!(c->flags & TKV_FLAG_HOVERED)) //if the hover flag is not raised
				{
					//Send hover signal and rise flag
					c->OnHoverEnter();
					c->flags |= TKV_FLAG_HOVERED;
				}

				if (select)
				{
					c->OnSelect();
					c->flags |= TKV_FLAG_SELECTED;
				}
			}
			else //Selection fails
			{
				if (c->flags & TKV_FLAG_HOVERED) //if hover flag is raised
				{
					//Send hover leave signal and lower flag
					c->OnHoverLeave();
					c->flags &= ~TKV_FLAG_HOVERED;
				}

				if (select && !multi && c->flags & TKV_FLAG_SELECTED) // if multi is not enabled and selected flag is raised
				{
					c->OnDeselect();
					c->flags &= ~TKV_FLAG_SELECTED;
				}
			}
		});

		/*
		SelectableComponent* s = nullptr;
		Float minMag = std::numeric_limits<Float>::max();

		auto hover = SelectableComponent::ForEach([mousePos, &s, &minMag](SelectableComponent* c)
		{
			Float mag = Distance(mousePos, c->transform->location);

			if (mag < c->hoverRadius && mag < minMag)
			{
				s = c;
				minMag = mag;
			}
			else
			{
				if (c->flags & TKV_FLAG_HOVERED)
				{
					c->flags &= ~TKV_FLAG_HOVERED;

					c->OnHoverLeave();
				}
			}
		});

		if (s)
		{
			if (!(s->flags & TKV_FLAG_HOVERED))
			{
				s->flags |= TKV_FLAG_HOVERED;
				s->OnHoverEnter();
			}
		}*/
	}
}

void SelectionSystem::Init()
{
	//EventBus<MouseSelectEvent>::Get().AddEventListener(OnSelect);
	EventBus<EarlyUpdateEvent>::Get().AddEventListener(OnEarlyUpdate);
}

void SelectionSystem::Destroy()
{
	//EventBus<MouseSelectEvent>::Get().RemoveEventListener(OnSelect);
	EventBus<EarlyUpdateEvent>::Get().RemoveEventListener(OnEarlyUpdate);
}

SelectableComponent::SelectableComponent(IEntity* e, const TransformComponent3D* const follow) :
	Component<SelectableComponent>(e),
	team(team),
	transform(TransformComponent3D::CreateComponent(e)),
	draw(DrawModelComponent3D::CreateComponent(e, transform, ContentManager<Model>::Get().GetContent("select_outline"))),
	follow(FollowComponent::CreateComponent(e, follow, transform))
{
	this->flags = 0;
	this->hoverRadius = 20.0f;
	this->follow->followType = TKV_FOLLOW_X | TKV_FOLLOW_Z;
	this->transform->location.y = -100.0f;
	this->transform->scale = Vector3(20.0f);
}

void SelectableComponent::OnDelete()
{
	this->transform->Delete();
	this->draw->Delete();
	this->follow->Delete();
}

void SelectableComponent::OnSelect()
{
	LogInfo("Selectable", "Selected, flag is now: " + std::to_string(this->flags));

	this->transform->location.y = 1.0f;
}

void SelectableComponent::OnDeselect()
{
	LogInfo("Selectable", "DeSelected, flag is now: " + std::to_string(this->flags));

	this->transform->location.y = -100.0f;
}

void SelectableComponent::OnHoverEnter()
{
	LogInfo("Selectable", "Hovered, flag is now: " + std::to_string(this->flags));

	this->transform->location.y = 1.0f;
}

void SelectableComponent::OnHoverLeave()
{
	LogInfo("Selectable", "UnHovered, flag is now: " + std::to_string(this->flags));

	if (!(this->flags & TKV_FLAG_SELECTED))
	{
		this->transform->location.y = -100.0f;
	}
}