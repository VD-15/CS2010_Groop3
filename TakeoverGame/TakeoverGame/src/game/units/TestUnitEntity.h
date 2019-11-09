#pragma once

#include "../Takover.h"
#include "../GameManager.h"
#include "../components/TeamComponent.h"
#include "../components/Selectable.h"
#include "../components/CommandReciever.h"
#include "../map/Capture.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	struct TestUnitEntity : public Entity<TestUnitEntity>
	{
		TestUnitEntity(Team t, Vector2 location);
		void OnDelete() override;

		TransformComponent2D* transform;
		DrawTextureComponent2D* draw;
		TeamComponent* team;
		CaptureContributorComponent* capture;
		SelectableComponent* select;
		CommandRecieverComponent* reciever;
	};
}
