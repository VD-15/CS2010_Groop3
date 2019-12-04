#pragma once
#include "../../engine/core/GameEntity.hpp"
using namespace vlk;

namespace tkv {
	struct MenuButtonEntity : public Entity<MenuButtonEntity> {
		MenuButtonEntity(Char type);
		void OnDelete() override;
		Char buttonType;

	};


}