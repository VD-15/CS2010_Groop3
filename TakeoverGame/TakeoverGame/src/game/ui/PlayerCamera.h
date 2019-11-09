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
		PlayerCameraComponent(IEntity* e, CameraComponent2D* camera, TransformComponent2D* transform);

		CameraComponent2D* camera;
		TransformComponent2D* transform;
	};

	struct PlayerCameraEntity : public Entity<PlayerCameraEntity>
	{
		PlayerCameraEntity();
		void OnDelete() override;

		TransformComponent2D* transform;
		CameraComponent2D* camera;
		PlayerCameraComponent* logic;
	};
}