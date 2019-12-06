#pragma once
#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/UIComponent.h"

using namespace vlk;

namespace tkv {
	namespace MainMenuSystem
	{
		void Init();
		void Destroy();
	}


	struct MainMenuComponent : public HeapAllocateComponent, Component<MainMenuComponent> {

		MainMenuComponent(IEntity* e, Vector2& v);

		Vector2& size;

	};

	struct ButtonComponent : public Component<ButtonComponent> {

		typedef void (*ButtonFun)();	

		ButtonComponent(IEntity* e, ButtonFun onClick, const std::string& name);
		DrawTextureComponent2D* draw;
		TransformComponent2D* transform;
		UIComponent* ui;
		
		ButtonFun OnClick;
	};


	struct MainMenuEntity : public Entity<MainMenuEntity>
	{
		
		MainMenuEntity();
		void OnDelete() override;
		DrawTextureComponent2D* draw;
		TransformComponent2D* transform;
		MainMenuComponent* logic;
		ButtonComponent* buttonPlay;
		ButtonComponent* buttonExit;


	};



}


