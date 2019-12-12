#include "Building.h"
#include "../../engine/core/ContentManager.hpp"


using namespace tkv;
/*
BuildingEntity::BuildingEntity()
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("factory"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
	this->draw->depth = tkv::DEPTH_BUILDING;
	//this->draw->flags = VLK_TRANSLUCENT_BIT;
	this->select->hoverRadius = 64.0f;
}
void BuildingEntity::OnDelete()
{
	this->select->Delete();
	this->capture->Delete();
	this->team->Delete();
	this->draw->Delete();
	this->transform->Delete();
}*/

FactoryEntity::FactoryEntity(const Vector3& location)
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawModelComponent3D>(this->transform, ContentManager<Model>::Get().GetContent("factory"));
	this->select = CreateComponent<SelectableComponent>(this->transform);
	this->team = CreateComponent<TeamComponent>(Team::Neutral);
	this->capture = CreateComponent<CaptureComponent>(this->transform, this->team);
	this->fabricator = CreateComponent<UnitFabricatorComponent>(this->transform, this->team, &Units::T1Air);

	this->transform->location = location;
}

void FactoryEntity::OnDelete()
{
	this->draw->Delete();
	this->transform->Delete();
}

QuarryEntity::QuarryEntity(const Vector3& location)
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawModelComponent3D>(this->transform, ContentManager<Model>::Get().GetContent("quarry"));
	this->select = CreateComponent<SelectableComponent>(this->transform);
	this->team = CreateComponent<TeamComponent>(Team::Neutral);
	this->capture = CreateComponent<CaptureComponent>(this->transform, this->team);

	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
	this->fabricate = CreateComponent<ResourceFabricatorComponent>();

	this->transform->location = location;
	this->transform->scale = Vector3(2.0f);
}

void QuarryEntity::OnDelete()
{
	this->draw->Delete();
	this->transform->Delete();

}