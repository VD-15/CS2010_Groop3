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

	typedef UInt ResourceID;
	constexpr ResourceID RESOURCE_STEEL =		0;
	constexpr ResourceID RESOURCE_OIL =			1;
	constexpr ResourceID RESOURCE_SILICONE =	2;
	constexpr ResourceID RESOURCE_RUBBER =		3;
	constexpr ResourceID RESOURCE_ALUMINIUM =	4;
	constexpr ResourceID RESOURCE_URANIUM =		5;

	struct InventoryComponent : public Component<InventoryComponent>, public HeapAllocateComponent
	{
		InventoryComponent(IEntity* e, const TeamComponent* team);

		void AddResource(ResourceID r, Double amount);
		Double GetResource(ResourceID r);

		const TeamComponent* const team;

	private:
		std::array<Double, 6> resources;
	};

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
