#include "QuarryEntity.h"
#include "../../engine/core/ContentManager.hpp"


using namespace tkv;

QuarryEntity::QuarryEntity()
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("quarry"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
    this->fabricate = CreateComponent<UnitFabricatorComponent>();

	this->draw->depth = tkv::DEPTH_BUILDING;

	this->select->hoverRadius = 64.0f;
}

void QuarryEntity::OnDelete()
{
	this->select->Delete();
	this->capture->Delete();
	this->team->Delete();
	this->draw->Delete();
	this->transform->Delete();
    this->fabricate->Delete();
}
