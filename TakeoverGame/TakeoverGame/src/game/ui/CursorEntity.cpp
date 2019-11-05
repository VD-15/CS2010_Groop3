#include "CursorEntity.h"

using namespace tkv;

CursorComponent::CursorComponent(IEntity* e) :
	Component<CursorComponent>(e)
{
	this->transform = TransformComponent2D::CreateComponent(e);
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform);
}

void CursorComponent::Delete()
{
	LogVerbose("CursorComponent", "Delete called");

	this->transform->Delete();
	this->draw->Delete();

	Component<CursorComponent>::Delete();
}