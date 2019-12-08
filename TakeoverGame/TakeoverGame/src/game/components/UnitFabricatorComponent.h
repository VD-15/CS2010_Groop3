#pragma once

#include "../Takover.h"
#include "../components/TeamComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

using namespace vlk;

namespace tkv {

    namespace FabricateSystem{
        void Init();
        void Destroy();
    }
    
    struct UnitFabricatorComponent : public Component<UnitFabricatorComponent>
    {
        UnitFabricatorComponent(IEntity* e, Team t);

        TeamComponent* team;
    }
}