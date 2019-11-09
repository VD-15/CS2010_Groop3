#include "Selectable.h"
#include "../ui/Cursor.h"
#include "../../engine/core/Mouse.h"

using namespace tkv;

namespace
{
	void OnSelect(MouseSelectEvent& ev)
	{
		Vector2 startPos(ev.cursor->selectStartPos);
		Vector2 endPos(ev.cursor->transform->location);

		auto x = std::minmax(startPos.x, endPos.x);
		auto y = std::minmax(startPos.y, endPos.y);

		Vector2 min(x.first, y.first);
		Vector2 max(x.second, y.second);

		auto select = SelectableComponent::ForEach([min, max](SelectableComponent* c)
		{
			if (c->transform->location.x > min.x &&
				c->transform->location.x < max.x &&
				c->transform->location.y > min.y &&
				c->transform->location.y < max.y)
			{
				c->flags |= TKV_FLAG_SELECTED;

				if (c->OnSelect) c->OnSelect();
			}
			else
			{
				c->flags &= !TKV_FLAG_SELECTED;

				if (c->OnDeselect) c->OnDeselect();
			}
		});
	}

	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		Vector2 mousePos = Mouse::GetCenteredPosition();

		SelectableComponent* s = nullptr;
		Float minMag = std::numeric_limits<Float>::max();

		auto hover = SelectableComponent::ForEach([mousePos, &s, &minMag](SelectableComponent* c)
		{
			Float mag = (mousePos - c->transform->location).Magnitude();

			if (mag < c->hoverRadius && mag < minMag)
			{
				s = c;
				minMag = mag;
			}
			else
			{
				if (c->flags & TKV_FLAG_HOVERED)
				{
					if (c->OnHoverLeave) c->OnHoverLeave();
				}
			}

			c->flags &= !TKV_FLAG_HOVERED;
		});

		if (s)
		{
			s->flags |= TKV_FLAG_HOVERED;

			if (s->OnHoverEnter) s->OnHoverEnter();
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

SelectableComponent::SelectableComponent(IEntity* e, const TransformComponent2D* const transform) :
	Component<SelectableComponent>(e),
	transform(transform)
{
	this->priority = 0;
	this->flags = 0b00000000;
	this->hoverRadius = 1.0f;

	this->OnSelect = nullptr;
	this->OnDeselect = nullptr;;
	this->OnHoverEnter = nullptr;
	this->OnHoverLeave = nullptr;
}