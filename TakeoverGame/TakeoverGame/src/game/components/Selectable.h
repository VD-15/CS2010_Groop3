#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawModelComponent.h"
#include "../../engine/components/UIComponent.h"
#include "../util/Raycast.h"

#include "TeamComponent.h"
#include "FollowComponent.h"

using namespace vlk;

namespace tkv
{
	namespace SelectionSystem
	{
		void Init();
		void Destroy();
	}

	constexpr Byte TKV_FLAG_SELECTED =	0b0000001;
	constexpr Byte TKV_FLAG_HOVERED =	0b0000010;

	struct SelectableComponent : public Component<SelectableComponent>
	{
		//Creates a Selectable component
		//follow => the transform this component is following
		//team   => the team that is allowed to select this object
		SelectableComponent(IEntity* e, const TransformComponent3D* const follow);
		void OnDelete() override;

		Byte flags;
		Float hoverRadius;

		const TeamComponent* const team;

		//Transform of the selectable outline
		//Seperate from the entity this component is attached to because of rotations & the like
		TransformComponent3D* const transform;
		DrawModelComponent3D* const draw;
		FollowComponent* const follow;

		void OnSelect();
		void OnDeselect();
		void OnHoverEnter();
		void OnHoverLeave();
	};
}
