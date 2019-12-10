#include "PlayerCamera.h"

#include "../../engine/core/Keyboard.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/VLKTime.h"
#include "../../engine/core/Window.h"

using namespace tkv;

namespace
{
	void OnUpdate(UpdateEvent& ev)
	{
		Vector3 dir = Vector3();

		constexpr Float horzMoveSpeed = 8.0f;
		constexpr Float vertMoveSpeed = 0.5f;

		if (Keyboard::IsKeyDown(Keys::W)) dir.z -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::S)) dir.z += 1.0f;
		if (Keyboard::IsKeyDown(Keys::A)) dir.x -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::D)) dir.x += 1.0f;

		if (Magnitude(dir) > 1.0f) Normalize(dir);
		dir *= VLKTime::DeltaTime<Float>() * horzMoveSpeed;


		Float vertDir = Mouse::GetScrollDelta().y * vertMoveSpeed;
		
		auto move = PlayerCameraComponent::ForEach([dir, vertDir](PlayerCameraComponent* c)
		{
			c->transform->location.y += vertDir;

			c->transform->location.y = std::clamp(c->transform->location.y, 3.0f, 50.0f); //vertical clamp

			c->transform->location += dir * c->transform->location.y / 3.0f;

			c->transform->location.x = std::clamp(c->transform->location.x, -512.0f, 512.0f); //Horizontal clamp
			c->transform->location.z = std::clamp(c->transform->location.z, -462.0f, 562.0f); //Lateral clamp

			c->camera3D->target = Rotate(-Vector3Z, c->transform->rotation) + c->transform->location;
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

PlayerCameraComponent::PlayerCameraComponent(IEntity* e, TransformComponent3D* transform, CameraComponent3D* camera3D) :
	Component(e)
{
	this->transform = transform;
	this->camera3D = camera3D;
}

PlayerCameraEntity::PlayerCameraEntity()
{
	this->transform2D = CreateComponent<TransformComponent2D>();
	this->transform3D = CreateComponent<TransformComponent3D>();
	this->camera2D = CreateComponent<CameraComponent2D>(transform2D);
	this->camera3D = CreateComponent<CameraComponent3D>(transform3D);
	this->logic = CreateComponent<PlayerCameraComponent>(transform3D, camera3D);
	
	this->transform3D->location = Vector3(0.0f, 40.0f, 0.0f);
	this->transform3D->rotation = AngleAxis(vlk::Pi / 4.0f, Vector3X);
}

void PlayerCameraEntity::OnDelete()
{
	this->logic->Delete();
	this->camera2D->Delete();
	this->camera3D->Delete();
	this->transform2D->Delete();
	this->transform3D->Delete();
}