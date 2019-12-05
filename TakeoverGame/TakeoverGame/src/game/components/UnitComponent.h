#pragma once

namespace tkv {

	namespace CombatSystem{
		void Init();
		void Destroy();
	}

	//sends the damage value of the unit to the unit that is being attacked
	struct AttackEvent : public Event{
		AttackEvent(IEntity* e, double damage);
		IEntity* target;
	}

	//decreased the health of the unit by the damage dealt to it
	struct DamagedEvent : public Event{
		DamagedEvent(double d);
	}

	struct UnitComponent : public Component<UnitComponent>
	{
		UnitComponent(IEntity* e, const ProtoUnit* p, Team t);
		void UpgradeUnit();
		// CheckForEnemies() maybe?

		int tier = 1;
		double health, damage, range, atkspd, speed, aggro;
		TeamComponent* team;
		bool foundEnemy;
	};

}
