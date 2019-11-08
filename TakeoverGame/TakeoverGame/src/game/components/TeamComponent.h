#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	struct TeamComponent : public Component<TeamComponent>
	{
		TeamComponent(IEntity* e);

		Byte team;
	};
}
