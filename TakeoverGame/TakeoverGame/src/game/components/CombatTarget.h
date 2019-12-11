#pragma once

#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawModelComponent.h"

#include "TeamComponent.h"

using namespace vlk;

namespace tkv
{
	namespace CombatSystem
	{
		void Init();
		void Destroy();
	}

	//Specifies anything that can recieve damage
	//This component will delete it's parent if it's health falls below zero
	struct CombatTargetComponent : public Component<CombatTargetComponent>
	{
		CombatTargetComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* team, Double maxHealth);

		const TransformComponent3D* const transform;
		const TeamComponent* const team;

		void DealDamage(Double amount);

		const Double maxHealth;
		Double currentHealth;
	};

}