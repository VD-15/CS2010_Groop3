#include "PlayerCamera.h"

#include "../../engine/core/Keyboard.h"
#include "../../engine/core/VLKTime.h"

using namespace tkv;

namespace
{
	void OnUpdate(UpdateEvent& ev)
	{
		Vector2 dir;

		if (Keyboard::IsKeyDown(Keys::W)) dir.y += 1.0f;
		if (Keyboard::IsKeyDown(Keys::S)) dir.y -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::A)) dir.x -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::D)) dir.x += 1.0f;

		if (dir.Magnitude() > 1.0f) dir.Normalize();
		dir *= VLKTime::DeltaTime<Float>() * 512.0f;

		auto move = PlayerCameraComponent::ForEach([dir](PlayerCameraComponent* c)
		{
			c->transform->location += dir;
		});
	}
}
void CameraSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void CameraSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

PlayerCameraComponent::PlayerCameraComponent(IEntity* e, CameraComponent2D* camera, TransformComponent2D* transform) :
	Component(e)
{
	this->camera = camera;
	this->transform = transform;
}

PlayerCameraEntity::PlayerCameraEntity()
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->camera = CreateComponent<CameraComponent2D>(transform);
	this->logic = CreateComponent<PlayerCameraComponent>(camera, transform);
}

void PlayerCameraEntity::Delete()
{
	this->logic->Delete();
	this->camera->Delete();
	this->transform->Delete();
}