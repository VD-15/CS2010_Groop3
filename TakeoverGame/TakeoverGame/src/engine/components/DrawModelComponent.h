#pragma once

#include "TransformComponent.h"

namespace vlk
{
	struct DrawModelComponent3D : public Component<DrawModelComponent3D>
	{
		DrawModelComponent3D(IEntity* e, const TransformComponent3D* transform, Model* model);
	};
}