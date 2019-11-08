#pragma once

#include "../GameManager.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv
{
	constexpr Byte TKV_FLAG_SELECTED;
	constexpr Byte TKV_FLAG_HOVERED;

	struct SelectableComponent : public Component<SelectableComponent>
	{
		SelectableComponent(IEntity* e, TransformComponent2D* transform);

		SByte priority;
		Byte flags;
		Float hoverRadius;

		TransformComponent2D* transform;
	};
}
