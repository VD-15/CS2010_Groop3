#include "Selectable.h"
#include "../ui/Cursor.h"
#include "../../engine/components/CameraComponent.h"
#include "../../engine/core/ContentManager.hpp"

using namespace tkv;

namespace
{
	void OnSelect(MouseSelectEvent& ev)
	{
		LogInfo("Selectable", "OnSelect");

		if (ev.isStart) return;

		Vector2 startPos(ev.cursor->selectStartPos);
		Vector2 endPos(CameraComponent2D::ACTIVE->GetMousePosition());

		auto x = std::minmax(startPos.x, endPos.x);
		auto y = std::minmax(startPos.y, endPos.y);

		Vector2 min(x.first, y.first);
		Vector2 max(x.second, y.second);

		std::vector<SelectableComponent*> selected;
		selected.reserve(SelectableComponent::GetCount());

		auto select = SelectableComponent::ForEach([min, max, endPos, &selected, &ev](SelectableComponent* c)
		{
			if (c->team->team == ev.cursor->team->team)
			{
				if ((c->transform->location.x > min.x &&
					c->transform->location.x < max.x &&
					c->transform->location.y > min.y &&
					c->transform->location.y < max.y) ||
					Magnitude(endPos - c->transform->location) < c->hoverRadius && selected.size() < 1)
				{
					selected.push_back(c);
				}
				else
				{
					c->flags &= !TKV_FLAG_SELECTED;

					c->OnDeselect();
				}
			}
		});

		Int maxPriority = 0;

		for (auto it = selected.begin(); it != selected.end(); it++)
		{
			SelectableComponent* c = *it;
			if (c->priority > maxPriority) maxPriority = c->priority;
		}

		for (auto it = selected.begin(); it != selected.end(); it++)
		{
			SelectableComponent* c = *it;

			if (c->priority == maxPriority)
			{
				c->flags |= TKV_FLAG_SELECTED;

				c->OnSelect();
			}
			else
			{
				c->flags &= !TKV_FLAG_SELECTED;

				c->OnDeselect();
			}
		}
	}

	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		Vector2 mousePos = CameraComponent2D::ACTIVE->GetMousePosition();

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
		}
	}
}

void SelectionSystem::Init()
{
	EventBus<MouseSelectEvent>::Get().AddEventListener(OnSelect);
	EventBus<EarlyUpdateEvent>::Get().AddEventListener(OnEarlyUpdate);
}

void SelectionSystem::Destroy()
{
	EventBus<MouseSelectEvent>::Get().RemoveEventListener(OnSelect);
	EventBus<EarlyUpdateEvent>::Get().RemoveEventListener(OnEarlyUpdate);
}

SelectableComponent::SelectableComponent(IEntity* e, const TransformComponent2D* const transform, const TeamComponent* const team) :
	Component<SelectableComponent>(e),
	transform(transform),
	team(team)
{
	this->priority = 0;
	this->flags = 0b00000000;
	this->hoverRadius = 1.0f;
	this->draw = nullptr;
}

void SelectableComponent::OnDelete()
{
	if (this->draw) this->draw->Delete();
}

void SelectableComponent::OnSelect()
{
	LogInfo("Selectable", "Selected, flag is now: " + std::to_string(this->flags));

	if (!this->draw)
	{
		this->draw = DrawTextureComponent2D::CreateComponent(this->GetParent(), this->transform, ContentManager<Texture2D>::Get().GetContent("select_outline"));
		this->draw->size = Vector2(this->hoverRadius * 2.0f);
	}
	else
	{
		this->draw->texture = ContentManager<Texture2D>::Get().GetContent("select_outline");
	}
}

void SelectableComponent::OnDeselect()
{
	LogInfo("Selectable", "DeSelected, flag is now: " + std::to_string(this->flags));

	if (this->draw)
	{
		if (this->flags & TKV_FLAG_HOVERED)
		{
			this->draw->texture = ContentManager<Texture2D>::Get().GetContent("hover_outline");
		}
		else
		{
			this->draw->Delete();
			this->draw = nullptr;
		}
	}
}

void SelectableComponent::OnHoverEnter()
{
	LogInfo("Selectable", "Hovered, flag is now: " + std::to_string(this->flags));

	if (!this->draw)
	{
		this->draw = DrawTextureComponent2D::CreateComponent(this->GetParent(), this->transform, ContentManager<Texture2D>::Get().GetContent("hover_outline"));
		this->draw->size = Vector2(this->hoverRadius * 2.0f);
	}
	else
	{
		this->draw->texture = ContentManager<Texture2D>::Get().GetContent("hover_outline");
	}
}

void SelectableComponent::OnHoverLeave()
{
	LogInfo("Selectable", "UnHovered, flag is now: " + std::to_string(this->flags));

	if (this->draw)
	{
		if (this->flags & TKV_FLAG_SELECTED)
		{
			this->draw->texture = ContentManager<Texture2D>::Get().GetContent("select_outline");
		}
		else
		{
			this->draw->Delete();
			this->draw = nullptr;
		}
	}
}