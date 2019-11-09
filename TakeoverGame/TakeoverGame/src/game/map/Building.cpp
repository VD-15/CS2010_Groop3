#include "Building.h"
#include "../../engine/core/ContentManager.hpp"


using namespace tkv;

BuildingEntity::BuildingEntity()
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("factory"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform);

	this->draw->depth = tkv::DEPTH_BUILDING;
	//this->draw->flags = VLK_TRANSLUCENT_BIT;

	this->select->hoverRadius = 64.0f;
}

void BuildingEntity::OnDelete()
{
	this->capture->Delete();
	this->team->Delete();
	this->draw->Delete();
	this->transform->Delete();
}