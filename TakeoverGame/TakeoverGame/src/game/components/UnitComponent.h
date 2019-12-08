#pragma once

#include "../Takover.h"
#include "../components/TeamComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

namespace tkv {

	namespace CombatSystem{
		void Init();
		void Destroy();
	}



	struct UnitComponent : public Component<UnitComponent>
	{
		UnitComponent(IEntity* e, const ProtoUnit* p, Team t);
		DamageEnemy(double damage);
		//void UpgradeUnit(); not used in mvp

		//int tier = 1;
		double health, damage, range, atkspd, speed, aggro;
		TeamComponent* team;
		TransformComponent3D* transform;
		double cooldown;
	};

}