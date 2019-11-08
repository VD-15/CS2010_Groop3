#include "Cursor.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/ContentManager.hpp"
#include "../../engine/core/VLKRandom.h"
#include "../../engine/core/VLKTime.h"
#include "../Takover.h"

using namespace tkv;

namespace
{
	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		Vector2 mousePos = Mouse::GetCenteredPosition();

		auto update = CursorComponent::ForEach([mousePos](CursorComponent* c)
		{
			c->ui->offset = mousePos;
			//c->transform->rotation += VLKTime::DeltaTime<Float>();
			//c->draw->color = Color(VLKRandom::GetRandom<UInt>());
			//c->draw->color.a = 1.0f;
		});
	}
}

void CursorSystem::Init()
{
	EventBus<EarlyUpdateEvent>::Get().AddEventListener(OnEarlyUpdate);
}

void CursorSystem::Destroy()
{
	EventBus<EarlyUpdateEvent>::Get().RemoveEventListener(OnEarlyUpdate);
}

CursorComponent::CursorComponent(IEntity* e) :
	Component<CursorComponent>(e)
{
	this->transform = TransformComponent2D::CreateComponent(e);
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform, ContentManager<Texture2D>::Get().GetContent("cursor"));
	this->ui = UIComponent::CreateComponent(e, transform);

	this->ui->dock = DockType::Center;
	this->draw->flags = VLK_SCREEN_RELATIVE_BIT;
	this->draw->origin.Set(0.0f, 1.0f);
	this->draw->depth = tkv::DEPTH_CURSOR;
}

void CursorComponent::Delete()
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

void CursorEntity::Delete()
{
	this->cursor->Delete();
}