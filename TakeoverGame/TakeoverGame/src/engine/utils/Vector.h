#pragma once

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	template <class T>
	class Pair final
	{
		public:
		Pair<T>();
		Pair<T>(T x, T y);
		Pair<T>(T arr[2]);
		virtual ~Pair<T>();

		Pair<T>(const Pair<T>& other);
		Pair<T>(Pair<T>&& other) noexcept = default;
		Pair<T>& operator=(const Pair<T>& other);

		inline Boolean operator==(const Pair<T>& v) const	{ return this->x == v.x && this->y == v.y; }
		inline Boolean operator!=(const Pair<T>& v) const	{ return this->x != v.x || this->y != v.y; }
		
		union
		{
			T x;
			T a;
			T start;
		};

		union
		{
			T y;
			T b;
			T end;
		};
	};
	
	class Vector2 final
	{
		public:
		Vector2();
		Vector2(Int x, Int y);
		Vector2(Float x, Float y);
		Vector2(Float xy);

		Vector2(const Vector2& other);
		Vector2(Vector2&& other) noexcept;
		Vector2& operator=(const Vector2& other);

		Float x;
		Float y;

		inline Boolean operator==(const Vector2& v) const	{ return this->x == v.x && this->y == v.y; }
		inline Boolean operator!=(const Vector2& v) const	{ return this->x != v.x || this->y != v.y; }

		inline Vector2 operator+ (const Vector2& v) const	{ return Vector2(this->x + v.x, this->y + v.y); }
		inline Vector2 operator- (const Vector2& v) const	{ return Vector2(this->x - v.x, this->y - v.y); }
		inline Vector2 operator* (Float f) const			{ return Vector2(this->x * f, this->y * f); }
		inline Vector2 operator/ (Float f) const			{ return Vector2(this->x / f, this->y / f); }
		inline Vector2 operator* (const Vector2& v) const	{ return Vector2(this->x * v.x, this->y * v.y); }

		inline Vector2 operator-() const { return Vector2(-this->x, -this->y); }

		inline Vector2& operator+=(const Vector2& v)		{ this->x += v.x; this->y += v.y; return *this; }
		inline Vector2& operator-=(const Vector2& v)		{ this->x -= v.x; this->y -= v.y; return *this; }
		inline Vector2& operator*=(Float f)					{ this->x *= f; this->y *= f; return *this; }
		inline Vector2& operator/=(Float f)					{ this->x /= f; this->y /= f; return *this; }
		inline Vector2& operator*=(const Vector2& v)		{ this->x *= v.x; this->y *= v.y; return *this; }

		void Set(Float x, Float y);
		void Rotate(Float angle);
		void RotateAround(const Vector2& origin, Float angle);
		void Normalize();
		void Normalize(Float magnitude);
		Float Magnitude() const;
		Float DotProduct(const Vector2& v) const;
		Float GetAngleBetween(const Vector2& v) const;
		Float GetProjection(const Vector2& target) const;
		Vector2 Perpendicular() const;
		void Round();

		static Vector2 Rotate(const Vector2& vector, Float angle);
		static Vector2 RotateAround(const Vector2& vector, const Vector2& origin, Float angle);
		static Vector2 Normalize(const Vector2& vector);
		static Float Magnitude(const Vector2& vector);
		static Float DotProduct(const Vector2& a, const Vector2& b);
		static Float GetAngleBetween(const Vector2& a, const Vector2& b);
		static Float GetProjection(const Vector2& vector, const Vector2& target);
		static Vector2 Perpendicular(const Vector2& v);
		static Vector2 Round(const Vector2& v);

		static const Vector2 UP;
		static const Vector2 DOWN;
		static const Vector2 LEFT;
		static const Vector2 RIGHT;
		static const Vector2 ONE;
	};

	class Vector3 final
	{
		public:
		Vector3();
		Vector3(Float x, Float y, Float z);
		Vector3(const Vector2& v);
		Vector3(const Vector2& xy, Float z);

		Vector3(const Vector3& other);
		Vector3(Vector3&& other) noexcept;
		Vector3& operator=(const Vector3& other);

		Float x;
		Float y;
		Float z;

		inline Boolean operator==(const Vector3& v) const { return this->x == v.x && this->y == v.y && this->z == v.z; }
		inline Boolean operator!=(const Vector3& v) const { return this->x != v.x || this->y != v.y || this->z != v.z; }

		inline Vector3 operator+ (const Vector3& v) const { return Vector3(this->x + v.x, this->y + v.y, this->z + v.z); }
		inline Vector3 operator- (const Vector3& v) const { return Vector3(this->x - v.x, this->y - v.y, this->z - v.z); }
		inline Vector3 operator* (const Float f) const { return Vector3(this->x * f, this->y * f, this->z * f); }
		inline Vector3 operator/ (const Float f) const { return Vector3(this->x / f, this->y / f, this->z / f); }

		inline Vector3& operator+=(const Vector3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
		inline Vector3& operator-=(const Vector3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
		inline Vector3& operator*=(const Float f) { this->x *= f; this->y *= f; this->z *= f; return *this; }
		inline Vector3& operator/=(const Float f) { this->x /= f; this->y /= f; this->z /= f; return *this; }

		void Set(Float x, Float y, Float z);
		void Normalize();
		void Normalize(Float magnitude);
		Float Magnitude() const;
		void Round();

		static Float AngleBetween(const Vector3& a, const Vector3& b);
		static Vector3 CrossProduct(const Vector3& a, const Vector3& b);
		static Float DotProduct(const Vector3& a, const Vector3& b);
		static Vector3 Normalize(const Vector3& v);
		static Float Magnitude(const Vector3& v);

		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;
		static const Vector3 FORWARD;
		static const Vector3 BACKWARD;
		static const Vector3 ONE;
		static const Vector3 IDENTITY;
	};

	struct Quaternion final
	{
		Quaternion();
		Quaternion(Float x, Float y, Float z, Float w);

		inline Boolean operator==(const Quaternion& q) const { return this->x == q.x && this->y == q.y && this->z == q.z && this->w == q.w; }
		inline Boolean operator!=(const Quaternion& q) const { return this->x != q.x || this->y != q.y || this->z != q.z || this->w != q.w; }

		inline Quaternion operator* (const Quaternion& q) const
		{
			return Quaternion(
				this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,  // i
				this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x,  // j
				this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w,  // k
				this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z   // 1
			);
		}

		inline Quaternion& operator* (const Quaternion& q)
		{
			Float x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;  // i
			Float y = this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x;  // j
			Float z = this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w;  // k
			Float w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;  // 1

			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;

			return *this;
		}

		static Quaternion AxisAngle(const Vector3& axis, Float angle);
		static Quaternion FromEuler(const Vector3& v);
		static Quaternion Conjugate(const Quaternion& q);
		static Vector3 Rotate(const Vector3& v, const Quaternion& q);
		static Vector3 RotateAround(const Vector3& origin, const Vector3& v, const Quaternion& q);

		//Don't touch these unless you know what quaternions do.

		Float x;
		Float y;
		Float z;
		Float w;
	};

	//Where are we using this, again?
	struct Vector4 final
	{
		Vector4();
		Vector4(Float x, Float y, Float z);
		Vector4(Float x, Float y, Float z, Float w);
		Vector4(const Vector2& xy);
		Vector4(const Vector2& xy, Float z);
		Vector4(const Vector2& xy, Float z, Float w);
		Vector4(const Vector3& xyz);
		Vector4(const Vector3& xyz, Float w);

		Vector4(const Vector4& other);
		Vector4(Vector4&& other) noexcept;
		Vector4& operator=(const Vector4& other);

		Float x;
		Float y;
		Float z;
		Float w;

		inline Boolean operator==(const Vector4& v) const { return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w; }
		inline Boolean operator!=(const Vector4& v) const { return this->x != v.x || this->y != v.y || this->z != v.z && this->w == v.w; }

		inline Vector4 operator+ (const Vector4& v) const { return Vector4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w); }
		inline Vector4 operator- (const Vector4& v) const { return Vector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w); }
		inline Vector4 operator* (const Float f) const { return Vector4(this->x * f, this->y * f, this->z * f, this->w * f); }
		inline Vector4 operator/ (const Float f) const { return Vector4(this->x / f, this->y / f, this->z / f, this->w / f); }

		inline Vector4& operator+=(const Vector4& v) { this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; return *this; }
		inline Vector4& operator-=(const Vector4& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; return *this; }
		inline Vector4& operator*=(const Float f) { this->x *= f; this->y *= f; this->z *= f; this->w *= f; return *this; }
		inline Vector4& operator/=(const Float f) { this->x /= f; this->y /= f; this->z /= f; this->w /= f; return *this; }
		
		static const Vector4 UP;
		static const Vector4 DOWN;
		static const Vector4 LEFT;
		static const Vector4 RIGHT;
		static const Vector4 FORWARD;
		static const Vector4 BACKWARD;
		static const Vector4 ONE;
		static const Vector4 IDENTITY;
	};
}