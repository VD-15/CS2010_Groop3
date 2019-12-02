#pragma once

namespace tkv {

	namespace CombatSystem{
		void Init();
		void Destroy();
	}



	struct UnitComponent : public Component<UnitComponent>
	{
		UnitComponent(IEntity* e, const ProtoUnit* p, Team t);

		double health, damage, range, atkspd, speed, aggro;
		TeamComponent* team;
	};

}
