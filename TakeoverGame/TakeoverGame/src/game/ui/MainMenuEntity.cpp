#include "MainMenuEntity.h"
#include "../../engine/core/ContentManager.hpp"

using namespace tkv;


MainMenuEntity::MainMenuEntity() {
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("unit"));


}
void MainMenuEntity::OnDelete() {


}
