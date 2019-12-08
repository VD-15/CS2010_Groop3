#pragma once

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

using namespace vlk;

namespace tkv
{
	namespace FollowSystem
	{
		void Init();
		void Destroy();
	}

	typedef Byte FollowType;

	constexpr FollowType TKV_FOLLOW_X = 0b00000001;
	constexpr FollowType TKV_FOLLOW_Y = 0b00000010;
	constexpr FollowType TKV_FOLLOW_Z = 0b00000100;

	struct FollowComponent : public Component<FollowComponent>
	{
		FollowComponent(IEntity* e, const TransformComponent3D* target, TransformComponent3D* follower);

		const TransformComponent3D* target;
		TransformComponent3D* const follower;

		FollowType followType;
	};
}