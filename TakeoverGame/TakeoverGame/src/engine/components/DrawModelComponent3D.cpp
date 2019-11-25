#include "DrawModelComponent.h"

using namespace vlk;

DrawModelComponent3D::DrawModelComponent3D(IEntity* e, const TransformComponent3D* transform, const Model* model) :
	Component<DrawModelComponent3D>(e)
{
	this->transform = transform;
	this->model = model;
}