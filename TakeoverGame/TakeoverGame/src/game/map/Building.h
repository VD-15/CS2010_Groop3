#pragma once

#include "../GameManager.h"
#include "../components/TeamComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	namespace BuildingSystem
	{
		void Init();
		void Destroy();
	}

	struct BuildingEntity : public Entity<BuildingEntity>
	{
		BuildingEntity();
		void Delete() override;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
	};
}
