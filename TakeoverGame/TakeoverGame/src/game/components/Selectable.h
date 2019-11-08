#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	constexpr Byte TKV_FLAG_SELECTED =	0b0000001;
	constexpr Byte TKV_FLAG_HOVERED =	0b0000010;

	struct SelectableComponent : public Component<SelectableComponent>
	{
		SelectableComponent(IEntity* e, TransformComponent2D* transform);

		SByte priority;
		Byte flags;
		Float hoverRadius;

		TransformComponent2D* transform;
	};
}
