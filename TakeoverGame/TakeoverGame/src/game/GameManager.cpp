#include "GameManager.h"
#include "../engine/core/ContentManager.hpp"
#include "../engine/components/LightComponent.h"
#include "../engine/graphics/Texture.h"
#include "../engine/graphics/Model.h"

#include "ui/PlayerCamera.h"
#include "ui/Cursor.h"
#include "map/GameMap.h"
#include "map/Building.h"
#include "components/Selectable.h"
#include "components/CommandReciever.h"
#include "units/TestUnitEntity.h"

using namespace tkv;
using namespace vlk;

namespace
{

}

void GameManager::Init()
{
	CameraSystem::Init();

	ContentManager<Texture2D>::Get().LoadContent("unit");

	ContentManager<Material>::Get().LoadContent("cube_tex");

	ContentManager<Model>::Get().LoadContent("cube");

	for (Int i = -1; i < 2; i++)
	{
		auto t = TransformComponent3D::CreateComponent(nullptr);
		t->location.z = -10.0f;
		t->location.x = i * 2.5f;
		//t->rotation *= Quaternion::AxisAngle(Vector3(1.0f, 0.0f, 0.0f), vlk::Pi / 3.0f) * Quaternion::AxisAngle(Vector3(0.0f, 1.0f, 0.0f), vlk::Pi / 3.0f);
		DrawModelComponent3D::CreateComponent(nullptr, t, ContentManager<Model>::Get().GetContent("cube"));
	}

	//PointLightComponent3D::CreateComponent(nullptr, t);

	auto c = PlayerCameraEntity::CreateEntity();
	//c->transform3D->location = Vector3(0.0f, 10.0f, -10.0f);
}

void GameManager::Destroy()
{

	ContentManager<Model>::Get().UnloadContent("cube");

	ContentManager<Material>::Get().UnloadContent("cube_tex");

	CameraSystem::Destroy();
}