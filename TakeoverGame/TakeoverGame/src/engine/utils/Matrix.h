#pragma once

#include "Vector.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

namespace vlk
{
	typedef glm::mat4 Matrix4;
	typedef glm::mat3 Matrix3;

	inline Matrix4 IdentityMatrix4() { return glm::identity<Matrix4>(); }
	inline Matrix4 CreateOrthographic(Float left, Float right, Float bottom, Float top, Float zNear, Float zFar) { return glm::ortho(left, right, top, bottom, zNear, zFar); }
	inline Matrix4 CreatePerspective(Float fov, Float aspect, Float zNear, Float zFar) { return glm::perspective(fov, aspect, zNear, zFar); }
	inline Matrix4 CreateInfPerspective(Float fov, Float aspect) { return glm::infinitePerspective(fov, aspect, -0.01f); }
	inline Matrix4 CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) { return glm::lookAt(position, target, up); }
	inline Matrix4 CreateRotation(const Quaternion& q) { return glm::toMat4(q); }
	inline Matrix4 CreateTranslation(const Vector3& v) { return glm::translate(v); }
	inline Matrix4 CreateScale(const Vector3& v) { return glm::scale(v); }

	/*
	class Matrix3
	{
		public:
		Matrix3();

		Matrix3(const Matrix3& other);
		Matrix3(Matrix3&& other) noexcept;
		Matrix3& operator=(const Matrix3& other);

		inline Float& At(UInt col, UInt row) { return data[col + row * 3]; }
		inline const Float& At(UInt col, UInt row) const { return data[col + row * 3]; }

		inline Float* Data() { return data; }
		inline const Float* Data() const { return data; }

		Boolean operator==(const Matrix3& other) const;
		Boolean operator!=(const Matrix3& other) const;

		Matrix3  operator* (const Matrix3& other) const;
		Matrix3& operator*=(const Matrix3& other);

		Matrix3  operator* (Float scale) const;
		Matrix3& operator*=(Float scale);

		Vector3 operator*(const Vector3& v) const;

		static Matrix3 CreateIdentity();
		static Matrix3 CreateTranslation(const Vector2& translation);
		static Matrix3 CreateScale(const Vector2& scale);
		static Matrix3 CreateRotation(const Float angle);

		private:
		Float data[9];
	};

	class Matrix4
	{
		public:
		Matrix4();
		Matrix4(const std::array<Float, 16> data);

		Matrix4(const Matrix4& other);
		Matrix4(Matrix4&& other) noexcept;
		Matrix4& operator=(const Matrix4& other);

		inline Float& At(ULong col, ULong row) { return data[col * 4 + row]; }
		inline const Float& At(ULong col, ULong row) const { return data[col * 4 + row]; }

		inline Float* Data() { return data.data(); }
		inline const Float* Data() const { return data.data(); }

		Boolean operator==(const Matrix4& other) const;
		Boolean operator!=(const Matrix4& other) const;

		Matrix4  operator* (const Matrix4& other) const;
		Matrix4& operator*=(const Matrix4& other);

		Matrix4  operator* (Float scale) const;
		Matrix4& operator*=(Float scale);

		Vector4 operator*(const Vector4& v) const;

		static Matrix4 CreateIdentity();
		static Matrix4 CreateTranslation(const Vector3& translation);
		static Matrix4 CreateScale(const Vector3& scale);
		static Matrix4 CreateRotationX(Float angle);
		static Matrix4 CreateRotationY(Float angle);
		static Matrix4 CreateRotationZ(Float angle);
		static Matrix4 CreateRotation(const Quaternion& q);
		static Matrix4 CreateLookAt(const Vector3& right, const Vector3& up, const Vector3& direction, const Vector3& position);
		static Matrix4 CreatePerspective(Float fov, Float aspect, Float far, Float near);
		static Matrix4 CreateOrthographic(Float right, Float left, Float top, Float bottom, Float near, Float far);

		private:
		std::array<Float, 16> data;
	};*/
}