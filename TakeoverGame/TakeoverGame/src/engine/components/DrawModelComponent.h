#pragma once

#include "TransformComponent.h"
#include "../graphics/Model.h"

namespace vlk
{
	struct DrawModelComponent3D : public Component<DrawModelComponent3D>
	{
		DrawModelComponent3D(IEntity* e, const TransformComponent3D* transform, const Model* model);

		const TransformComponent3D* transform;
		const Model* model;
	};
}