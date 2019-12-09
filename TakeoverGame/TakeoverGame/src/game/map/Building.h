#pragma once

#include "../Takover.h"
#include "../GameManager.h"
#include "../components/TeamComponent.h"
#include "../components/Selectable.h"
#include "../map/Capture.h"
#include "../components/ResourceFabricatorComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawModelComponent.h"

using namespace vlk;

namespace tkv
{
	/*
	struct BuildingEntity : public Entity<BuildingEntity>
	{
		BuildingEntity();
		void OnDelete() override;
		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
		TeamComponent* team;
		CaptureComponent* capture;
		SelectableComponent* select;
	};*/

	struct FactoryEntity : public HeapAllocateEntity, public Entity<FactoryEntity>
	{
		FactoryEntity(const Vector3& location);
		void OnDelete() override;

		TransformComponent3D* transform;
		DrawModelComponent3D* draw;
		TeamComponent* team;
		CaptureComponent* capture;
		SelectableComponent* select;
		UnitFabricatorComponent* fabricate;
	};

	struct QuarryEntity : public HeapAllocateEntity, Entity<QuarryEntity>
	{
		QuarryEntity(const Vector3& quarry);
		void OnDelete() override;

		TransformComponent3D* transform;
		DrawModelComponent3D* draw;
		TeamComponent* team;
		CaptureComponent* capture;
		SelectableComponent* select;
		ResourceFabricatorComponent* fabricate;
	};
}
