#include "GameManager.h"
#include "../engine/core/ContentManager.hpp"
#include "../engine/graphics/Texture.h"

#include "ui/PlayerCamera.h"
#include "ui/Cursor.h"
#include "map/GameMap.h"
#include "map/Building.h"
#include "components/Selectable.h"

using namespace tkv;
using namespace vlk;

namespace
{

}

void GameManager::Init()
{
	CursorSystem::Init();
	CameraSystem::Init();
	SelectionSystem::Init();

	ContentManager<Texture2D>::Get().LoadContent("cursor");
	ContentManager<Texture2D>::Get().LoadContent("map");
	ContentManager<Texture2D>::Get().LoadContent("factory");
	ContentManager<Texture2D>::Get().LoadContent("debug");

	//Create a camera entity and make it's camera active
	PlayerCameraEntity::CreateEntity()->camera->Activate();
	CursorEntity::CreateEntity();
	MapEntity::CreateEntity();
	BuildingEntity::CreateEntity();
}

void GameManager::Destroy()
{
	SelectionSystem::Destroy();
	CameraSystem::Destroy();
	CursorSystem::Destroy();

	ContentManager<Texture2D>::Get().UnloadContent("debug");
	ContentManager<Texture2D>::Get().UnloadContent("cursor");
	ContentManager<Texture2D>::Get().UnloadContent("map");
	ContentManager<Texture2D>::Get().UnloadContent("factory");
}