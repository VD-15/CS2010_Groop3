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
		CommandIssuedEvent(Vector3 target);

		const Vector3 target;
	};

	struct CommandRecieverComponent : public Component<CommandRecieverComponent>
	{
		CommandRecieverComponent(IEntity* e, TransformComponent3D* const transform, const SelectableComponent* const select);

		TransformComponent3D* const transform;
		const SelectableComponent* const select;

		Vector3 target;
		Boolean isMoving;
		Float speed;
	};
}