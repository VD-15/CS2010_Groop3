#pragma once

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/DrawModelComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	namespace UnitSystem
	{
		void Init();
		void Destroy();
	}

	struct ProtoUnit
	{
		Float health;
		Float attackRange;
		Float viewRange;

		Float speed;
		const std::string modelName;
	};

	struct UnitEntity : public Entity<UnitEntity>
	{
		UnitEntity(const Vector3& position, const ProtoUnit& p);
		void OnDelete() override;

		TransformComponent3D* const transform;
		DrawModelComponent3D* const draw;
	};
}