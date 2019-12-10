#pragma once
0
#include "../Takover.h"
#include "../components/TeamComponent.h"
#include "../components/UnitComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

using namespace vlk;

namespace tkv {

    namespace FabricatorSystem{
        void Init();
        void Destroy();
    }

    struct UnitFabricatorComponent : public Component<UnitFabricatorComponent>
    {
        UnitFabricatorComponent(IEntity* e, TransformComponent3D transform, Team t, ProtoUnit* unit = meleeUnit);
        ChangeUnit(ProtoUnit* unit);

        ProtoUnit* unit;
        TransformComponent3D transform;
        TeamComponent* team;
        double cooldown;
    }

    struct ResourceFabricatorComponent : public Component<ResourceFabricatorComponent>
    {
        ResourceFabricatorComponent(IEntity* e, Team t);

        TeamComponent* team;
        double cooldown;
    }
}
