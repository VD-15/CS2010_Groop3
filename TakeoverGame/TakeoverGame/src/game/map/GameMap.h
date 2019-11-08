#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	struct MapEntity : public Entity<MapEntity>, public HeapAllocateEntity
	{
		MapEntity();
		void Delete() override;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
	};
}
