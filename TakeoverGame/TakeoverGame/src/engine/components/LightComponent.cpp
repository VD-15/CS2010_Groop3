#include "LightComponent.h"

using namespace vlk;

ILight::ILight() :
	color(Color::WHITE)
{
	constant = 1.0f;
	linear = 1.0f;
	quadratic = 1.0f;
}

const AmbientLightComponent3D* AmbientLightComponent3D::ACTIVE = nullptr;

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

}