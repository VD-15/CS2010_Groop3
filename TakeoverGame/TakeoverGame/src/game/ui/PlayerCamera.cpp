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

		if (Keyboard::IsKeyDown(Keys::W)) dir.z += 1.0f;
		if (Keyboard::IsKeyDown(Keys::S)) dir.z -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::A)) dir.x -= 1.0f;
		if (Keyboard::IsKeyDown(Keys::D)) dir.x += 1.0f;
		dir.y += Mouse::GetScrollDelta().y;

		if (Magnitude(dir) > 1.0f) Normalize(dir);
		dir *= VLKTime::DeltaTime<Float>();

		auto move = PlayerCameraComponent::ForEach([dir](PlayerCameraComponent* c)
		{
			c->transform->location += dir;
			Window::SetTitle(std::to_string(c->transform->location.z));
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

PlayerCameraComponent::PlayerCameraComponent(IEntity* e, TransformComponent3D* transform) :
	Component(e)
{
	this->transform = transform;
}

PlayerCameraEntity::PlayerCameraEntity()
{
	this->transform2D = CreateComponent<TransformComponent2D>();
	this->transform3D = CreateComponent<TransformComponent3D>();
	this->camera2D = CreateComponent<CameraComponent2D>(transform2D);
	this->camera3D = CreateComponent<CameraComponent3D>(transform3D);
	this->logic = CreateComponent<PlayerCameraComponent>(transform3D);

	//this->transform3D->location = Vector3(0.0f, 0.0f, 0.0f);
	//this->transform3D->rotation = Quaternion::AxisAngle(Vector3::RIGHT, vlk::PiOverTwo);
}

void PlayerCameraEntity::OnDelete()
{
	this->logic->Delete();
	this->camera2D->Delete();
	this->camera3D->Delete();
	this->transform2D->Delete();
	this->transform3D->Delete();
}