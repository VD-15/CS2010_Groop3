#pragma once

#include "../Takover.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"

#include "Selectable.h"

namespace tkv
{
	namespace CommandRecieverSystem
	{
		void Init();
		void Destroy();
	}

	struct CommandIssuedEvent : public Event
	{
		CommandIssuedEvent(Vector2 target);

		const Vector2 target;
	};

	struct CommandRecieverComponent : public Component<CommandRecieverComponent>
	{
		CommandRecieverComponent(IEntity* e, TransformComponent2D* const transform, const SelectableComponent* const select);

		TransformComponent2D* const transform;
		const SelectableComponent* const select;

		Vector2 target;
		Boolean isMoving;
		Float speed;
	};
	
}
