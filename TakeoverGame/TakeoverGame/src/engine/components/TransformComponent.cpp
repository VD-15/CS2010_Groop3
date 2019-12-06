#include "TransformComponent.h"

using namespace vlk;

TransformComponent2D::TransformComponent2D(IEntity* e) : 
	Component<TransformComponent2D>(e), 
	location(), 
	scale(1.0f)
{
	rotation = 0.0f;
}

void TransformComponent2D::Translate(Vector2 translation)
{
	this->location += translation;
}

void TransformComponent2D::Rotate(Float rotation)
{
	this->rotation += rotation;
}

TransformComponent3D::TransformComponent3D(IEntity* e) :
	Component<TransformComponent3D>(e),
	location(),
	scale(1.0f),
	rotation(Identity())
{

}

void TransformComponent3D::TranslateLocal(const Vector3& translation)
{
	this->location += this->rotation * translation;
}

void TransformComponent3D::TranslateGlobal(const Vector3& translation)
{
	this->location += translation;
}

void TransformComponent3D::Rotate(const Vector3& axis, Float angle)
{
	this->location = this->location * AngleAxis(angle, axis);
}

void TransformComponent3D::Rotate(const Quaternion& q)
{
	this->location = this->location * q;
}