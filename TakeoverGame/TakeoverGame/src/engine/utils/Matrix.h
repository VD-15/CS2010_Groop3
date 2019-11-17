#pragma once

#include "Vector.h"
#include "../core/ValkyrieEngine.h"

#include <array>

namespace vlk
{
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

		inline Float& At(ULong col, ULong row) { return data[col + row * 4]; }
		inline const Float& At(ULong col, ULong row) const { return data[col + row * 4]; }

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
		static Matrix4 CreatePerspective(Float left, Float right, Float top, Float bottom, Float near, Float far);
		static Matrix4 CreateOrthographic(Float right, Float left, Float top, Float bottom, Float near, Float far);

		private:
		std::array<Float, 16> data;
	};
}