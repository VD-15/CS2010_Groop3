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
#include "../../engine/components/DrawModelComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	namespace UnitSystem
	{
		void Init();
		void Destroy();
	}
	struct ProtoUnit {
		double health, damage, range, atkspd, speed;
	}
	constexpr ProtoUnit meleeUnit {
		10, 4, 1, 1, 1
	};
	constexpr ProtoUnit rangedUnit {
		8, 4, 5, 2, 1
	};
	constexpr ProtoUnit flyingUnit {
		6, 2, 5, 1, 2
	};
	constexpr ProtoUnit commander{
		500, 10, 3, 1, 1
	}
	struct UnitEntity : public Entity<UnitEntity>
	{
		UnitEntity(Team t, Vector2 location, ProtoUnit p);
		void onDelete() override;

		TransformComponent3D* transform;
		DrawModelComponent3D* draw;
		TeamComponent* team;
		CaptureContributorComponent* capture;
		SelectableComponent* select;
		CommandRecieverComponent* reciever;
		UnitComponent* logic;
	};
}
