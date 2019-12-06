#pragma once

#include "../components/TransformComponent.h"
#include "../utils/Matrix.h"

namespace vlk
{
	namespace Camera
	{
		void Init();
		void Destroy();
	}

	struct CameraComponent2D : public Component<CameraComponent2D>
	{
		//The active camera
		static const CameraComponent2D* ACTIVE;

		CameraComponent2D(IEntity* e, TransformComponent2D* transform);

		void Activate();

		Vector2 GetMousePosition() const;
		Matrix4 GetProjection() const;
		Matrix4 GetView() const;

		Boolean autoResize;
		Vector2 viewport;
		Vector2 zoom;
		const TransformComponent2D* const transform;
	};

	struct CameraComponent3D : public Component<CameraComponent3D>
	{
		static const CameraComponent3D* ACTIVE;

		CameraComponent3D(IEntity* e, TransformComponent3D* transform);

		void Activate();

		Matrix4 GetProjection() const;
		Matrix4 GetView() const;

		Boolean autoResize;

		//Feild of view is measured in degrees for readability purposes
		Float fov;
		Vector2 viewport;
		Vector3 target;

		const TransformComponent3D* const transform;
	};
}