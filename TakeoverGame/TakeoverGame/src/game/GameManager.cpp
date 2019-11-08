#include "GameManager.h"
#include "../engine/core/ContentManager.hpp"
#include "../engine/graphics/Texture.h"
#include "ui/PlayerCamera.h"
#include "ui/Cursor.h"
#include "map/GameMap.h"

using namespace tkv;
using namespace vlk;

namespace
{

}

void GameManager::Init()
{
	CursorSystem::Init();
	CameraSystem::Init();

	ContentManager<Texture2D>::Get().LoadContent("cursor");
	ContentManager<Texture2D>::Get().LoadContent("map");

	//Create a camera entity and make it's camera active
	PlayerCameraEntity::CreateEntity()->camera->Activate();
	CursorEntity::CreateEntity();
	MapEntity::CreateEntity();
}

void GameManager::Destroy()
{
	CameraSystem::Destroy();
	CursorSystem::Destroy();

	ContentManager<Texture2D>::Get().UnloadContent("cursor");
	ContentManager<Texture2D>::Get().UnloadContent("map");
}