#pragma once

#include "../../engine/utils/Vector.h"

using namespace vlk;

namespace tkv
{
	struct Ray
	{
		Ray(const Vector3& direction = Vector3(0.0f, 0.0f, 1.0f), const Vector3& location = Vector3(0.0f, 0.0f, 0.0f));

		Vector3 location;
		Vector3 direction;
	};

	struct IVolume
	{
		virtual Bool Intersects(const Ray& r) = 0;
	};

	//Circle volume lies on the X/Z plane
	struct CircleVolume : public IVolume
	{
		CircleVolume(const Vector3& location = Vector3(0.0f, 0.0f, 0.0f), Float radius = 1.0f);

		Bool Intersects(const Ray& r) override;

		Vector3 location;
		Float radius;
	};
}