#pragma once

#include "../core/GameComponent.hpp"
#include "../utils/Vector.h"

namespace vlk
{
	struct TransformComponent2D : public Component<TransformComponent2D>
	{
		TransformComponent2D(IEntity* e);

		void Translate(Vector2 translation);
		void Rotate(Float rotation);

		Vector2 location;
		Vector2 scale;
		Float rotation;
	};
	
	struct TransformComponent3D : public Component<TransformComponent3D>
	{
		TransformComponent3D(IEntity* e);

		void TranslateLocal(const Vector3& translation);
		void TranslateGlobal(const Vector3& translation);
		void Rotate(const Vector3& axis, Float angle);
		void Rotate(const Quaternion& q);

		Vector3 location;
		Vector3 scale;
		Quaternion rotation;
	};
	
}