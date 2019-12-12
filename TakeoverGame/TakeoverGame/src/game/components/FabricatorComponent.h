#pragma once

#include "../Takover.h"
#include "../components/TeamComponent.h"
#include "../units/Unit.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

using namespace vlk;

namespace tkv
{

	namespace FabricatorSystem
	{
		void Init();
		void Destroy();
	}

	struct UnitFabricatorComponent : public Component<UnitFabricatorComponent>
	{
		UnitFabricatorComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* t, const ProtoUnit* unit);

		void ChangeUnit(const ProtoUnit* unit);

		//The protounit this factory is making
		const ProtoUnit* production;

		const TransformComponent3D* const transform;
		const TeamComponent* const team;

		Double cooldown;
		Vector3 spawnLocation;
	};
}
