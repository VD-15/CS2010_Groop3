#pragma once

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawModelComponent.h"
#include "../../engine/components/UIComponent.h"

#include "../components/TeamComponent.h"
#include "../components/Selectable.h"
#include "../components/CommandReciever.h"
#include "../components/CombatTarget.h"
#include "../map/Capture.h"

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
		const Double health;
		const Double attackRange;
		const Double attackDamage;
		const Double attackSpeed;

		const Double moveSpeed;

		const Bool isCommander;

		const char* modelName;
	};

	constexpr ProtoUnit T1Ground
	{
		10.0,
		512.0,
		1.0,
		1.0,

		64.0,
		false,

		"unit"
	};

	constexpr ProtoUnit Commander
	{
		100.0,
		512.0,
		5.0,
		0.5,

		16.0,

		"commander"
	};

	//Deals damage to any opposing combat targets in range.
	struct UnitComponent : public Component<UnitComponent>
	{
		UnitComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* team, const ProtoUnit* prototype);

		const TransformComponent3D* const transform;
		const TeamComponent* const team;
		const ProtoUnit* const prototype;
	};

	struct UnitEntity : public Entity<UnitEntity>
	{
		UnitEntity(const Vector3& position, const ProtoUnit* p, Team team);
		void OnDelete() override;

		TransformComponent3D* const transform;
		DrawModelComponent3D* const draw;
		TeamComponent* const team;
		SelectableComponent* const select;
		CaptureContributorComponent* const capture;
		CommandRecieverComponent* const reciever;

		CombatTargetComponent* const target;
		UnitComponent* const logic;
	};
}