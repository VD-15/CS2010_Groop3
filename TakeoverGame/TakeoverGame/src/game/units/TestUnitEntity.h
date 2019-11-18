#pragma once

#include "../Takover.h"
#include "../GameManager.h"
#include "../components/TeamComponent.h"
#include "../components/Selectable.h"
#include "../components/CommandReciever.h"
#include "../map/Capture.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/DrawModelComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	namespace TestSystem
	{
		void Init();
		void Destroy();
	}

	struct TestComponent : public Component<TestComponent>
	{
		TestComponent(IEntity* e, TransformComponent3D* transform);

		TransformComponent3D* const transform;
	};

	struct TestUnitEntity : public Entity<TestUnitEntity>
	{
		TestUnitEntity(Team t, Vector2 location);
		void OnDelete() override;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
		TeamComponent* team;
		CaptureContributorComponent* capture;
		SelectableComponent* select;
		CommandRecieverComponent* reciever;
	};

	struct TestUnitEntity2 : public Entity<TestUnitEntity2>
	{
		TestUnitEntity2(const Vector3& location);
		void OnDelete() override;

		TransformComponent3D* transform;
		TestComponent* test;
		DrawModelComponent3D* draw;
	};
}
