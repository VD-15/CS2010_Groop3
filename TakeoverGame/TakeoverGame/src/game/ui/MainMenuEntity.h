#pragma once
#include "../GameManager.h"
#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/TransformComponent.h"
using namespace vlk;

namespace tkv {

	struct MainMenuEntity : public Entity<MainMenuEntity>
	{
		MainMenuEntity();
		void OnDelete() override;
		DrawTextureComponent2D* draw;
		TransformComponent2D* transform;
	}



}


