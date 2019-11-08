#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	namespace CursorSystem
	{
		void Init();
		void Destroy();
	}

	struct CursorComponent : public Component<CursorComponent>, public HeapAllocateComponent
	{
		CursorComponent(IEntity* e);
		void Delete() override;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
		UIComponent* ui;
	};

	struct CursorEntity : public Entity<CursorEntity>, public HeapAllocateEntity
	{
		CursorEntity();
		void Delete() override;

		CursorComponent* cursor;
	};
}
