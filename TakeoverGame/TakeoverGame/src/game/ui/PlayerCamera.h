#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/CameraComponent.h"
#include "../../engine/components/DrawModelComponent.h"

using namespace vlk;

namespace tkv
{
	namespace CameraSystem
	{
		void Init();
		void Destroy();
	}

	struct CameraFollowTest : public Component<CameraFollowTest>
	{
		CameraFollowTest(IEntity* e, const CameraComponent3D* camera, TransformComponent3D* transform);

		const CameraComponent3D* camera;
		TransformComponent3D* transform;
	};

	struct CameraFollowEntity : public Entity<CameraFollowEntity>
	{
		CameraFollowEntity(const CameraComponent3D* camera);
		void OnDelete() override;

		TransformComponent3D* transform;
		DrawModelComponent3D* draw;
		CameraFollowTest* follow;
	};

	struct PlayerCameraComponent : public Component<PlayerCameraComponent>
	{
		PlayerCameraComponent(IEntity* e, TransformComponent3D* transform, CameraComponent3D* camera3D);

		TransformComponent3D* transform;
		CameraComponent3D* camera3D;
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

		//IEntity* follow;
	};
}