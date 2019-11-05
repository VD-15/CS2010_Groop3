#pragma once

#include "Vector.h"
#include "../core/ValkyrieEngine.h"

namespace vlk
{
	class Matrix3
	{
		public:
		Matrix3();

		Matrix3(const Matrix3& other);
		Matrix3(Matrix3&& other) noexcept;
		Matrix3& operator=(const Matrix3& other);

		virtual ~Matrix3();

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

		Matrix4(const Matrix4& other);
		Matrix4(Matrix4&& other) noexcept;
		Matrix4& operator=(const Matrix4& other);

		virtual ~Matrix4();

		inline Float& At(UInt col, UInt row) { return data[col + row * 4]; }
		inline const Float& At(UInt col, UInt row) const { return data[col + row * 4]; }

		inline Float* Data() { return data; }
		inline const Float* Data() const { return data; }

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
		static Matrix4 CreateLookAt(Vector3 right, Vector3 up, Vector3 direction, Vector3 position);
		static Matrix4 CreatePerspective(Float left, Float right, Float top, Float bottom, Float near, Float far);
		static Matrix4 CreateOrthographic(Float right, Float left, Float top, Float bottom, Float near, Float far);

		private:
		Float data[16];
	};
}