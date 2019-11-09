#include "DrawTextureComponent.h"

using namespace vlk;

DrawTextureComponent2D::DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture) :
	Component<DrawTextureComponent2D>(e),
	DrawBaseComponent2D(transform)
{
	this->transform = transform;
	this->texture = texture;
	this->size.Set(static_cast<Float>(texture->Width()), static_cast<Float>(texture->Height()));
	this->origin.Set(0.5f, 0.5f);
}