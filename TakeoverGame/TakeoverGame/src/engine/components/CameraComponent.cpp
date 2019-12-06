#include "CameraComponent.h"
#include "../core/Window.h"
#include "../../engine/core/Mouse.h"

using namespace vlk;

namespace
{
	void OnResize(vlk::WindowFramebufferEvent& ev)
	{
		auto resize2D = CameraComponent2D::ForEach([ev](CameraComponent2D* c)
		{
			if (c->autoResize)
			{
				c->viewport.x = ev.width / 2.0f;
				c->viewport.y = ev.height / 2.0f;
			}
		});

		auto resize3D = CameraComponent3D::ForEach([ev](CameraComponent3D* c)
		{
			if (c->autoResize)
			{
				c->viewport.x = ev.width / 2.0f;
				c->viewport.y = ev.height / 2.0f;
			}
		});
	}
}

void Camera::Init()
{
	EventBus<WindowFramebufferEvent>::Get().AddEventListener(OnResize);
}

void Camera::Destroy()
{
	EventBus<WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);
}

const CameraComponent2D* CameraComponent2D::ACTIVE = nullptr;
const CameraComponent3D* CameraComponent3D::ACTIVE = nullptr;

CameraComponent2D::CameraComponent2D(IEntity* e, TransformComponent2D* transform) :
	Component(e),
	transform(transform)
{
	this->autoResize = true;
	this->zoom = Vector2(1.0f);

	if (!CameraComponent2D::ACTIVE)
	{
		CameraComponent2D::ACTIVE = this;
	}
}

void CameraComponent2D::Activate()
{
	CameraComponent2D::ACTIVE = this;
}

Vector2 CameraComponent2D::GetMousePosition() const
{
	return Mouse::GetCenteredPosition() + this->transform->location;
}

Matrix4 CameraComponent2D::GetProjection() const
{
	return CreateOrthographic(	viewport.x *  (zoom.x), 
								-viewport.x * (zoom.x), 
								viewport.y *  (zoom.y), 
								-viewport.y * (zoom.y), 
								-1048576.0f, 
								1048576.0f);
}

Matrix4 CameraComponent2D::GetView() const
{
	//return Matrix4::CreateLookAt(this->transform->location, this->transform->location + Vector3::FORWARD);
	return IdentityMatrix4();
}

CameraComponent3D::CameraComponent3D(IEntity* e, TransformComponent3D* transform) : 
	Component<CameraComponent3D>(e),
	transform(transform),
	viewport(1.0f)
{
	this->fov = 90.0f;
	this->autoResize = true;

	if (!CameraComponent3D::ACTIVE)
	{
		CameraComponent3D::ACTIVE = this;
	}
}

void CameraComponent3D::Activate()
{
	CameraComponent3D::ACTIVE = this;
}

Matrix4 CameraComponent3D::GetProjection() const
{
	Float aspect = viewport.y / viewport.x;
	Float radians = fov * (vlk::Pi / 180.0f);
	
	return CreatePerspective(radians, aspect, 0.01f, 100.0f);
	//return CreateInfPerspective(radians, aspect);
}

Matrix4 CameraComponent3D::GetView() const
{
	Vector3 up(Vector3Y);
	Vector3 forward(Rotate(-Vector3Z, this->transform->rotation));

	return CreateLookAt(this->transform->location, Vector3(0.0f, 0.0f, -10.0f), Vector3Y);
}