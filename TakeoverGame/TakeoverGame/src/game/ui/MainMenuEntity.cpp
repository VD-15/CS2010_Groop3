#include "MainMenuEntity.h"
#include "../../engine/core/ContentManager.hpp"
#include "../../engine/core/Window.h"
#include "../../engine/core/Application.h"
#include "../../engine/core/Mouse.h"

using namespace tkv;

namespace
{
	void OnResize(vlk::WindowFramebufferEvent& ev)
	{
		MainMenuComponent::ForEach([ev](MainMenuComponent* c)
			{
				c->size = Vector2(ev.width, ev.height);
			});
	}
	void OnUpdate(vlk::UpdateEvent& ev)
	{
		if (Mouse::IsButtonPressed(MouseButtons::BUTTON_LEFT))
		{
			Vector2 mousePs = Mouse::GetCenteredPosition();

			ButtonComponent::ForEach([mousePs](ButtonComponent* c)
				{
					Vector2 componentPs = c->transform->location;
					if (mousePs.x > componentPs.x - c->draw->size.x / 2.0f &&
						mousePs.x < componentPs.x + c->draw->size.x / 2.0f &&
						mousePs.y > componentPs.y - c->draw->size.y / 2.0f &&
						mousePs.y < componentPs.y + c->draw->size.y / 2.0f
						) {
						c->OnClick();
					}

				});
		}
	
	
	}

	void Play()
	{
		
		LogAnnounce("Button", "PLAY!");
	}

	void Exit()
	{
		Application::Exit();
	}
}

void MainMenuSystem::Init()
{
	EventBus<WindowFramebufferEvent>::Get().AddEventListener(OnResize);
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void MainMenuSystem::Destroy()
{
	EventBus<WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);
}

ButtonComponent::ButtonComponent(IEntity* e, ButtonFun onClick, const std::string& name) :
	Component<ButtonComponent>(e)
{
	this->transform = TransformComponent2D::CreateComponent(e);
	this->draw = DrawTextureComponent2D::CreateComponent(e, this->transform, ContentManager<Texture2D>::Get().GetContent(name));
	this->ui = UIComponent::CreateComponent(e, this->transform);

	this->draw->depth = -1.0f;

	this->draw->flags = VLK_SCREEN_RELATIVE_BIT;

	this->OnClick = onClick;
}

MainMenuComponent::MainMenuComponent(IEntity* e, Vector2& v) :
	Component<MainMenuComponent>(e),
	size(v)
{

}

MainMenuEntity::MainMenuEntity() {
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("MainMenu"));
	this->logic = CreateComponent<MainMenuComponent>(draw->size);
	auto size = vlk::Window::GetSize();
	this->buttonPlay = CreateComponent<ButtonComponent>(::Play, "PlayButton");
	this->buttonExit = CreateComponent<ButtonComponent>(::Exit, "ExitButton");

	this->buttonExit->ui->offset = Vector2(0.0f, -100.0f);

	this->draw->size = Vector2(size.x, size.y);
}
void MainMenuEntity::OnDelete() {
	this->transform->Delete();
	this->draw->Delete();
	this->logic->Delete();
	this->buttonPlay->transform->Delete();
	this->buttonPlay->draw->Delete();
	this->buttonPlay->ui->Delete();
    this->buttonExit->transform->Delete();
	this->buttonExit->draw->Delete();
	this->buttonExit->ui->Delete();
}
