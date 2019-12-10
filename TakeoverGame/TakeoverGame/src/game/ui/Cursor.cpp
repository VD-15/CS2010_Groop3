#include "Cursor.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/ContentManager.hpp"
#include "../../engine/core/VLKRandom.h"
#include "../../engine/core/VLKTime.h"
#include "../../engine/components/CameraComponent.h"
#include "../Takover.h"
#include "../components/CommandReciever.h"

using namespace tkv;

namespace
{
	void OnMouseSelect(MouseSelectEvent& ev)
	{
		if (ev.isStart)
		{
			MouseSelectionBoxEntity::CreateEntity(ev.cursor);
		}
		else
		{
			std::vector<IEntity*> v;
			v.reserve(MouseSelectionBoxComponent::GetCount());

			MouseSelectionBoxComponent::ForEach([&v](MouseSelectionBoxComponent* c)
			{
				v.push_back(c->GetParent());
			});

			for (auto it = v.begin(); it != v.end(); it++)
			{
				IEntity* e = *it;
				e->Delete();
			}
		}
	}

	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		Vector2 mousePos = Mouse::GetCenteredPosition();

		auto update = CursorComponent::ForEach([mousePos](CursorComponent* c)
		{
			c->ui->offset = mousePos;

			if (Mouse::IsButtonPressed(MouseButtons::BUTTON_LEFT))
			{
				c->selectStartPos = CameraComponent2D::ACTIVE->GetMousePosition();
				EventBus<MouseSelectEvent>::Get().PostEvent({ true, c });
			}
			else if (Mouse::IsButtonReleased(MouseButtons::BUTTON_LEFT))
			{
				EventBus<MouseSelectEvent>::Get().PostEvent({ false, c });
			}

			if (Mouse::IsButtonPressed(MouseButtons::BUTTON_RIGHT))
			{
				EventBus<CommandIssuedEvent>::Get().PostEvent(CameraComponent2D::ACTIVE->GetMousePosition());
			}

			//c->transform->rotation += VLKTime::DeltaTime<Float>();
			//c->draw->color = Color(VLKRandom::GetRandom<UInt>());
			//c->draw->color.a = 1.0f;
		});
	}

	void OnUpdate(UpdateEvent& ev)
	{
		Vector2 mousePos(CameraComponent2D::ACTIVE->GetMousePosition());

		auto update = MouseSelectionBoxComponent::ForEach([mousePos](MouseSelectionBoxComponent* c)
		{
			Vector2 size = (mousePos - c->cursor->selectStartPos);
			c->transform->location = c->cursor->selectStartPos + size * 0.5f;
			c->draw->size = size;
		});
	}
}

void CursorSystem::Init()
{
	EventBus<MouseSelectEvent>::Get().AddEventListener(OnMouseSelect);
	EventBus<EarlyUpdateEvent>::Get().AddEventListener(OnEarlyUpdate);
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void CursorSystem::Destroy()
{
	EventBus<MouseSelectEvent>::Get().RemoveEventListener(OnMouseSelect);
	EventBus<EarlyUpdateEvent>::Get().RemoveEventListener(OnEarlyUpdate);
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

InventoryComponent::InventoryComponent(IEntity* e, const TeamComponent* team) :
	Component<InventoryComponent>(e),
	team(team)
{

}
void InventoryComponent::AddResource(ResourceID r, Double amount) {
	this->resources[r] += amount;

}
Double InventoryComponent::GetResource(ResourceID r) {
	return this->resources[r];
}

MouseSelectEvent::MouseSelectEvent(Boolean isStart, const CursorComponent* const cursor) :
	cursor(cursor),
	isStart(isStart)
{
	
}

CursorComponent::CursorComponent(IEntity* e) :
	Component<CursorComponent>(e)
{
	this->transform = TransformComponent2D::CreateComponent(e);
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform, ContentManager<Texture2D>::Get().GetContent("cursor"));
	this->ui = UIComponent::CreateComponent(e, transform);
	this->team = TeamComponent::CreateComponent(e);

	this->ui->dock = DockType::Center;
	this->draw->flags = VLK_SCREEN_RELATIVE_BIT;
	this->draw->origin = Vector2(0.0f, 1.0f);
	this->draw->depth = tkv::DEPTH_CURSOR;

	this->team->team = Team::Team1;
}

void CursorComponent::OnDelete()
{
	LogVerbose("CursorComponent", "Delete called");

	this->transform->Delete();
	this->draw->Delete();

	Component<CursorComponent>::Delete();
}

CursorEntity::CursorEntity()
{
	this->cursor = CreateComponent<CursorComponent>();
}

void CursorEntity::OnDelete()
{
	this->cursor->Delete();
}

MouseSelectionBoxComponent::MouseSelectionBoxComponent(IEntity* e, const CursorComponent* const cursor) :
	Component<MouseSelectionBoxComponent>(e),
	cursor(cursor)
{
	this->transform = TransformComponent2D::CreateComponent(e);
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform, ContentManager<Texture2D>::Get().GetContent("debug"));

	this->draw->color = Color(0.0f, 0.75f, 0.0f, 0.5f);
	this->draw->flags = VLK_TRANSLUCENT_BIT;
	this->draw->depth = tkv::DEPTH_CURSOR;
}

void MouseSelectionBoxComponent::OnDelete()
{
	this->draw->Delete();
	this->transform->Delete();
}

MouseSelectionBoxEntity::MouseSelectionBoxEntity(const CursorComponent* const cursor)
{
	this->logic = CreateComponent<MouseSelectionBoxComponent>(cursor);
}

void MouseSelectionBoxEntity::OnDelete()
{
	this->logic->Delete();
}
