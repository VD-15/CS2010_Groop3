#include "GameMap.h"
#include "../../engine/core/ContentManager.hpp"
#include "../Takover.h"

using namespace tkv;

MapEntity::MapEntity()
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(this->transform, ContentManager<Texture2D>::Get().GetContent("map"));

	this->draw->depth = tkv::DEPTH_MAP;
}

void MapEntity::OnDelete()
{
	this->draw->Delete();
	this->transform->Delete();
}