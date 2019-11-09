#include "TransformComponent.h"

using namespace vlk;

TransformComponent2D::TransformComponent2D(IEntity* e) : 
	Component<TransformComponent2D>(e), 
	location(), 
	scale(Vector2::ONE)
{
	rotation = 0.0f;
}

void TransformComponent2D::Translate(Vector2 translation)
{
	this->location += translation;
}

void TransformComponent2D::Scale(Vector2 scale)
{
	this->scale.x += scale.x;
	this->scale.y += scale.y;
}

void TransformComponent2D::Scale(Float scale)
{
	this->scale *= scale;
}

void TransformComponent2D::Rotate(Float rotation)
{
	this->rotation += rotation;
}