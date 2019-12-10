#include "DrawTextureComponent.h"

using namespace vlk;

DrawTextureComponent2D::DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture) :
	Component<DrawTextureComponent2D>(e),
	DrawBaseComponent2D(transform),
	size(static_cast<Float>(texture->Width()), static_cast<Float>(texture->Height())),
	origin(0.5f, 0.5f)
{
	this->transform = transform;
	this->texture = texture;
}