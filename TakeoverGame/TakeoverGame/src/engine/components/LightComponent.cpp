#include "LightComponent.h"

using namespace vlk;

ILight::ILight() :
	color(Color::WHITE)
{
	constant = 1.0f;
	linear = 0.0f;
	quadratic = 0.0f;
}

const AmbientLightComponent3D* AmbientLightComponent3D::ACTIVE = nullptr;
const DirectionLightComponent3D* DirectionLightComponent3D::ACTIVE = nullptr;

AmbientLightComponent3D::AmbientLightComponent3D(IEntity* e) :
	Component<AmbientLightComponent3D>(e)
{
	if (!AmbientLightComponent3D::ACTIVE)
	{
		AmbientLightComponent3D::ACTIVE = this;
	}
}

void AmbientLightComponent3D::Activate()
{
	AmbientLightComponent3D::ACTIVE = this;
}

PointLightComponent3D::PointLightComponent3D(IEntity* e, const TransformComponent3D* transform) :
	Component<PointLightComponent3D>(e),
	transform(transform)
{

}

DirectionLightComponent3D::DirectionLightComponent3D(IEntity* e, const TransformComponent3D* transform) :
	Component<DirectionLightComponent3D>(e),
	transform(transform)
{
	if (!DirectionLightComponent3D::ACTIVE)
	{
		DirectionLightComponent3D::ACTIVE = this;
	}
}

void DirectionLightComponent3D::Activate()
{
	DirectionLightComponent3D::ACTIVE = this;
}