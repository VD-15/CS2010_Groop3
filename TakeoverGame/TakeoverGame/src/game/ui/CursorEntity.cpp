#include "CursorEntity.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/ContentManager.h"

using namespace tkv;

namespace
{
	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		Vector2 mousePos = Mouse::GetPosition();

		auto update = CursorComponent::ForEach([mousePos](CursorComponent* c)
		{
			c->transform->location = mousePos;
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
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform, ContentManager::GetContent<Texture2D>("cursor"));
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