#pragma once

#include "../Takover.h"
#include "../components/TeamComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

using namespace vlk;

namespace tkv {
    
    struct ResourceFabricatorComponent : public Component<ResourceFabricatorComponent>
    {
        ResourceFabricatorComponent(IEntity* e, Team t);

        TeamComponent* team;
    }
}