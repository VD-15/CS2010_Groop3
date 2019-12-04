#pragma once
#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/TransformComponent.h"
#include"../../game/ui/MenuButtonEntity.h"
using namespace vlk;

namespace tkv {

	struct MainMenuEntity : public Entity<MainMenuEntity>
	{
		MainMenuEntity();
		void OnDelete() override;
		DrawTextureComponent2D* draw;
		TransformComponent2D* transform;
		MenuButtonEntity* buttonPlay;
		MenuButtonEntity* buttonExit;
	};



}


