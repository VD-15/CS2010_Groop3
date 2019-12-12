#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	enum class Team
	{
		Neutral,
		Team1,
		Team2
	};

	struct TeamComponent : public Component<TeamComponent>
	{
		TeamComponent(IEntity* e, Team t = Team::Neutral);

		Team team;
	};
}
