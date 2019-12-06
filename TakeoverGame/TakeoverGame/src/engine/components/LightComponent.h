#pragma once

#include "TransformComponent.h"
#include "../utils/Color.h"

namespace vlk
{
	struct ILight
	{
		ILight();

		//intensity = (quadratic * distance^2) + (linear * distance) + constant

		Float constant;
		Float linear;
		Float quadratic;

		Color color;
	};

	struct AmbientLightComponent3D : public ILight, public HeapAllocateComponent, public Component<AmbientLightComponent3D>
	{
		static const AmbientLightComponent3D* ACTIVE;

		AmbientLightComponent3D(IEntity* e);

		void Activate();
	};

	struct PointLightComponent3D : public ILight, public Component<PointLightComponent3D>
	{
		PointLightComponent3D(IEntity* e, const TransformComponent3D* transform);

		const TransformComponent3D* const transform;
	};

	struct DirectionLightComponent3D : public ILight, public Component<DirectionLightComponent3D>
	{
		static const DirectionLightComponent3D* ACTIVE;

		DirectionLightComponent3D(IEntity* e, const TransformComponent3D* transform);

		const TransformComponent3D* const transform;

		void Activate();
	};
}