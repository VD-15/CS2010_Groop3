#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/CameraComponent.h"

using namespace vlk;

namespace tkv
{
	namespace CameraSystem
	{
		void Init();
		void Destroy();
	}

	struct PlayerCameraComponent : public Component<PlayerCameraComponent>
	{
		PlayerCameraComponent(IEntity* e, TransformComponent3D* transform);

		TransformComponent3D* transform;
	};

	struct PlayerCameraEntity : public Entity<PlayerCameraEntity>
	{
		PlayerCameraEntity();
		void OnDelete() override;

		TransformComponent3D* transform3D;
		TransformComponent2D* transform2D;
		CameraComponent3D* camera3D;
		CameraComponent2D* camera2D;
		PlayerCameraComponent* logic;
	};
}