#include "PlayerCamera.h"

#include "../../engine/core/Keyboard.h"
#include "../../engine/core/Mouse.h"
#include "../../engine/core/VLKTime.h"
#include "../../engine/core/Window.h"

#include "../../engine/core/ContentManager.hpp"

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
			/*
			Window::SetTitle(std::to_string(c->camera3D->transform->location.x) + ", " +
							 std::to_string(c->camera3D->transform->location.y) + ", " +
							 std::to_string(c->camera3D->transform->location.z));*/
		});

		Vector2 mousePos(Mouse::GetCenteredPosition());
		mousePos = Vector2(mousePos.x / (Window::GetWidth() / 2.0f), mousePos.y / (Window::GetHeight() / 2.0f));

		auto follow = CameraFollowTest::ForEach([mousePos](CameraFollowTest* c)
		{
			//c->transform->location = c->camera->transform->location - Rotate(Vector3(-mousePos.x, -mousePos.y, 1.0f) * 2.0f, c->camera->transform->rotation);
			
			Matrix4 inv(glm::inverse(c->camera->GetProjection()));// * CreateRotation(c->camera->transform->rotation)));

			Vector4 v(mousePos, 0.0f, 1.0f);
			
			Vector3 dir(inv * v);
			dir = Rotate(dir, c->camera->transform->rotation);
			dir += c->camera->transform->location;

			c->transform->location = dir;
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

CameraFollowTest::CameraFollowTest(IEntity* e, const CameraComponent3D* camera, TransformComponent3D* transform) :
	Component<CameraFollowTest>(e)
{
	this->camera = camera;
	this->transform = transform;
}

CameraFollowEntity::CameraFollowEntity(const CameraComponent3D* camera)
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawModelComponent3D>(this->transform, ContentManager<Model>::Get().GetContent("cube"));
	this->follow = CreateComponent<CameraFollowTest>(camera, this->transform);

	this->transform->scale = Vector3(0.1f);
}

void CameraFollowEntity::OnDelete()
{
	this->transform->Delete();
	this->draw->Delete();
	this->follow->Delete();
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

	//this->follow = dynamic_cast<IEntity*>(CameraFollowEntity::CreateEntity(this->camera3D));
}

void PlayerCameraEntity::OnDelete()
{
	this->logic->Delete();
	this->camera2D->Delete();
	this->camera3D->Delete();
	this->transform2D->Delete();
	this->transform3D->Delete();

	//this->follow->Delete();
}