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
	ContentManager<Texture2D>::Get().LoadContent("map_snow");

	ContentManager<Material>::Get().LoadContent("cube_tex");
	ContentManager<Material>::Get().LoadContent("map_snow");
	ContentManager<Material>::Get().LoadContent("brick");
	ContentManager<Material>::Get().LoadContent("black");
	ContentManager<Material>::Get().LoadContent("glass");
	ContentManager<Material>::Get().LoadContent("quarry");

	ContentManager<Model>::Get().LoadContent("cube");
	ContentManager<Model>::Get().LoadContent("map_snow");
	ContentManager<Model>::Get().LoadContent("factory");
	ContentManager<Model>::Get().LoadContent("quarry");

	{
		auto t = TransformComponent3D::CreateComponent(nullptr);
		auto l = DirectionLightComponent3D::CreateComponent(nullptr, t);
		t->location = Normalize(Vector3(0.0f, 1.0f, -0.5f));
		l->constant = 0.5f;
		l->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	{
		auto t = TransformComponent3D::CreateComponent(nullptr);
		auto d = DrawModelComponent3D::CreateComponent(nullptr, t, ContentManager<Model>::Get().GetContent("map_snow"));

		t->scale = Vector3(512.0f, 1.0f, 512.0f);
	}

	{
		auto f = FactoryEntity::CreateEntity(Vector3(0.0f, 0.0f, 0.0f));
		f->transform->rotation = AngleAxis(vlk::PiOverTwo, Vector3Y);

		auto q1 = QuarryEntity::CreateEntity(Vector3(448.0f, 0.0f, 448.0f));
		auto q2 = QuarryEntity::CreateEntity(Vector3(-448.0f, 0.0f, -448.0f));
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