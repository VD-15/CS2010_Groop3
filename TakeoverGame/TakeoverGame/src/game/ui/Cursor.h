#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

#include "../components/TeamComponent.h"

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
		void OnDelete() override;

		Vector2 selectStartPos;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
		UIComponent* ui;
		TeamComponent* team;
	};

	struct MouseSelectEvent : public Event
	{
		MouseSelectEvent(Boolean isStart, const CursorComponent* const cursor);

		const Boolean isStart;
		const CursorComponent* const cursor;
	};

	struct CursorEntity : public Entity<CursorEntity>, public HeapAllocateEntity
	{
		CursorEntity();
		void OnDelete() override;

		CursorComponent* cursor;
	};

	struct MouseSelectionBoxComponent : public Component<MouseSelectionBoxComponent>, public HeapAllocateComponent
	{
		MouseSelectionBoxComponent(IEntity* e, const CursorComponent* const cursor);
		void OnDelete() override;

		const CursorComponent* const cursor;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
	};

	struct MouseSelectionBoxEntity : public Entity<MouseSelectionBoxEntity>, public HeapAllocateEntity
	{
		MouseSelectionBoxEntity(const CursorComponent* const cursor);
		void OnDelete() override;

		MouseSelectionBoxComponent* logic;
	};
}
