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
#include "ui/MainMenuEntity.h"

using namespace tkv;
using namespace vlk;

namespace
{

}

void GameManager::Init()
{
	TestSystem::Init();
	CursorSystem::Init();
	CameraSystem::Init();
	CaptureSystem::Init();
	SelectionSystem::Init();
	CommandRecieverSystem::Init();
	MainMenuSystem::Init();

	ContentManager<Texture2D>::Get().LoadContent("MainMenu");
	ContentManager<Texture2D>::Get().LoadContent("cursor");
	ContentManager<Texture2D>::Get().LoadContent("map");
	ContentManager<Texture2D>::Get().LoadContent("factory");
	ContentManager<Texture2D>::Get().LoadContent("debug");
	ContentManager<Texture2D>::Get().LoadContent("select_outline");
	ContentManager<Texture2D>::Get().LoadContent("hover_outline");
	ContentManager<Texture2D>::Get().LoadContent("unit");
	ContentManager < Texture2D>::Get().LoadContent("PlayButton");
	ContentManager < Texture2D>::Get().LoadContent("ExitButton");

	ContentManager<Texture2D>::Get().LoadContent("cap_0");
	ContentManager<Texture2D>::Get().LoadContent("cap_1");
	ContentManager<Texture2D>::Get().LoadContent("cap_2");
	ContentManager<Texture2D>::Get().LoadContent("cap_3");
	ContentManager<Texture2D>::Get().LoadContent("cap_4");
	ContentManager<Texture2D>::Get().LoadContent("cap_5");
	ContentManager<Texture2D>::Get().LoadContent("cap_6");
	ContentManager<Texture2D>::Get().LoadContent("cap_7");
	ContentManager<Texture2D>::Get().LoadContent("cap_8");
	ContentManager<Texture2D>::Get().LoadContent("cap_9");
	ContentManager<Texture2D>::Get().LoadContent("cap_10");

	ContentManager<Material>::Get().LoadContent("monkey_green");
	ContentManager<Material>::Get().LoadContent("monkey_blue");
	ContentManager<Material>::Get().LoadContent("monkey_yellow");
	//ContentManager<Model>::Get().LoadContent("monkey_hd");

	
	auto e = MainMenuEntity::CreateEntity();

	

	//Create a camera entity and make it's camera active
	//PlayerCameraEntity::CreateEntity()->camera->Activate();
	//CursorEntity::CreateEntity();
	//MapEntity::CreateEntity();
	//BuildingEntity::CreateEntity();

	//TestUnitEntity::CreateEntity(Team::Team1, Vector2(64.0f, 0.0f));
	//TestUnitEntity::CreateEntity(Team::Team1, Vector2(-64.0f, 0.0f));
	/*
	TestUnitEntity2::CreateEntity(Vector3( 0.0f, 0.0f, 5.0f));
	TestUnitEntity2::CreateEntity(Vector3(-5.0f, 0.0f, 5.0f));
	TestUnitEntity2::CreateEntity(Vector3( 5.0f, 0.0f, 5.0f));
	*/
	auto c = CameraComponent3D::CreateComponent(nullptr, TransformComponent3D::CreateComponent(nullptr));
	CameraComponent2D::CreateComponent(nullptr, TransformComponent2D::CreateComponent(nullptr));
	/*
	const_cast<TransformComponent3D*>(c->transform)->location.z += 2.0f;

	//AmbientLightComponent3D::CreateComponent(nullptr)->color = Color(0.05f, 0.05f, 0.05f, 1.0f);

	{
		TransformComponent3D* t1 = TransformComponent3D::CreateComponent(nullptr);
		t1->location.x = -2.0f;
		t1->location.y = 0.0f;
		t1->location.z = 5.0f;
		PointLightComponent3D::CreateComponent(nullptr, t1);// ->color = Color::GREEN;

		TransformComponent3D* t2 = TransformComponent3D::CreateComponent(nullptr);
		t2->location.x = 2.0f;
		t2->location.y = -0.0f;
		t2->location.z = 5.0f;
		PointLightComponent3D::CreateComponent(nullptr, t2);// ->color = Color::RED;
	}

	constexpr UInt startingUnits = 8;
	Float inc = vlk::TwoPi / startingUnits;

	for (UInt i = 0; i < startingUnits; i++)
	{
		Vector2 p1(Vector2::RotateAround(Vector2(448.0f, 480.0f), Vector2(480.0f, 480.0f), inc * static_cast<Float>(i)));

		//TestUnitEntity::CreateEntity(Team::Team2, p1);
		//TestUnitEntity::CreateEntity(Team::Team1, -p1);
	}*/
}

void GameManager::Destroy()
{
	MainMenuSystem::Destroy();
	CommandRecieverSystem::Destroy();
	SelectionSystem::Destroy();
	CaptureSystem::Destroy();
	CameraSystem::Destroy();
	CursorSystem::Destroy();
	TestSystem::Destroy();

	ContentManager<Material>::Get().UnloadContent("monkey_green");
	ContentManager<Material>::Get().UnloadContent("monkey_blue");
	ContentManager<Material>::Get().UnloadContent("monkey_yellow");
	ContentManager<Model>::Get().UnloadContent("monkey_hd");

	ContentManager<Texture2D>::Get().UnloadContent("cap_0");
	ContentManager<Texture2D>::Get().UnloadContent("cap_1");
	ContentManager<Texture2D>::Get().UnloadContent("cap_2");
	ContentManager<Texture2D>::Get().UnloadContent("cap_3");
	ContentManager<Texture2D>::Get().UnloadContent("cap_4");
	ContentManager<Texture2D>::Get().UnloadContent("cap_5");
	ContentManager<Texture2D>::Get().UnloadContent("cap_6");
	ContentManager<Texture2D>::Get().UnloadContent("cap_7");
	ContentManager<Texture2D>::Get().UnloadContent("cap_8");
	ContentManager<Texture2D>::Get().UnloadContent("cap_9");
	ContentManager<Texture2D>::Get().UnloadContent("cap_10");

	ContentManager<Texture2D>::Get().UnloadContent("unit");
	ContentManager<Texture2D>::Get().UnloadContent("hover_outline");
	ContentManager<Texture2D>::Get().UnloadContent("select_outline");
	ContentManager<Texture2D>::Get().UnloadContent("debug");
	ContentManager<Texture2D>::Get().UnloadContent("cursor");
	ContentManager<Texture2D>::Get().UnloadContent("map");
	ContentManager<Texture2D>::Get().UnloadContent("factory");
}