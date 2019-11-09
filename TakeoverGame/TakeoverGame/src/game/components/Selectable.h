#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

#include "TeamComponent.h"

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
		SelectableComponent(IEntity* e, const TransformComponent2D* const transform, const TeamComponent* team);
		void OnDelete() override;

		SByte priority;
		Byte flags;
		Float hoverRadius;
		
		const TransformComponent2D* const transform;
		const TeamComponent* const team;

		DrawTextureComponent2D* draw;

		void OnSelect();
		void OnDeselect();
		void OnHoverEnter();
		void OnHoverLeave();
	};
}
