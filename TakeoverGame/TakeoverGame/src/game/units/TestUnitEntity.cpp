#include "TestUnitEntity.h"

#include "../../engine/core/ContentManager.hpp"

using namespace tkv;

TestUnitEntity::TestUnitEntity(Team t, Vector2 location)
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("unit"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureContributorComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
	this->reciever = CreateComponent<CommandRecieverComponent>(transform, select);

	this->transform->location = location;
	this->team->team = t;

	this->draw->depth = tkv::DEPTH_UNIT;
	this->draw->size = 32.0f;

	this->reciever->speed = 64.0f;
	this->select->hoverRadius = 32.0f;
	this->select->priority = 1;
}

void TestUnitEntity::OnDelete()
{
	reciever->Delete();
	select->Delete();
	capture->Delete();
	team->Delete();
	draw->Delete();
	transform->Delete();
}