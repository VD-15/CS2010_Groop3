#include "CameraComponent.h"
#include "../core/Window.h"
#include "../../engine/core/Mouse.h"

using namespace vlk;

namespace
{
	void OnResize(vlk::WindowFramebufferEvent& ev)
	{
		auto resize = CameraComponent2D::ForEach([ev](CameraComponent2D* c)
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

CameraComponent2D::CameraComponent2D(IEntity* e, TransformComponent2D* transform) :
	Component(e)
{
	this->transform = transform;
	this->autoResize = true;
	this->zoom = 1.0f;

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
	return Matrix4::CreateOrthographic(	viewport.x *  (zoom.x), 
										-viewport.x * (zoom.x), 
										viewport.y *  (zoom.y), 
										-viewport.y * (zoom.y), 
										-1048576.0f, 
										1048576.0f);
}

Matrix4 CameraComponent2D::GetView() const
{
	return Matrix4::CreateLookAt(Vector3::RIGHT, Vector3::UP, Vector3::FORWARD, Vector3(this->transform->location, 0.0f));
}