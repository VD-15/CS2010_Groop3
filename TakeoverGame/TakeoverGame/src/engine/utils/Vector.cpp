#include "Vector.h"
#include <cmath>
#include <functional>

using namespace vlk;

template<class T>
Pair<T>::Pair()
{
	this->x = T();
	this->y = T();
}

template<class T>
Pair<T>::Pair(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<class T>
Pair<T>::Pair(T arr[2])
{
	this->x = arr[0];
	this->y = arr[1];
}

template<class T>
Pair<T>::~Pair() {}

template<class T>
Pair<T>::Pair(const Pair<T>& other)
{
	this->x = other.x;
	this->y = other.y;
}

template<class T>
Pair<T>& Pair<T>::operator=(const Pair<T>& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

template class Pair<SByte>;
template class Pair<Byte>;
template class Pair<Short>;
template class Pair<UShort>;
template class Pair<Int>;
template class Pair<UInt>;
template class Pair<Long>;
template class Pair<ULong>;
template class Pair<Float>;
template class Pair<Double>;

Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

Vector2::Vector2(Int x, Int y)
{
	this->x = static_cast<Float>(x);
	this->y = static_cast<Float>(y);
}

Vector2::Vector2(Float x, Float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(Float xy)
{
	this->x = xy;
	this->y = xy;
}

Vector2::Vector2(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
}

Vector2::Vector2(Vector2&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
}

Vector2& Vector2::operator=(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

void Vector2::Set(Float x, Float y)
{
	this->x = x;
	this->y = y;
}

void Vector2::Rotate(Float angle)
{
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	float newX = x * cosa - y * sina;
	float newY = x * sina + y * cosa;

	x = newX;
	y = newY;
}

void Vector2::RotateAround(const Vector2& origin, Float angle)
{
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	float xDiff = x - origin.x;
	float yDiff = y - origin.y;

	float newX = cosa * xDiff - sina * yDiff + origin.x;
	float newY = sina * xDiff + cosa * yDiff + origin.y;

	x = newX;
	y = newY;
}

void Vector2::Normalize()
{
	float mag = sqrtf((x * x) + (y * y));

	x /= mag;
	y /= mag;
}

void Vector2::Normalize(Float magnitude)
{
	float mag = sqrtf((x * x) + (y * y));

	float diff = magnitude / mag;

	x *= diff;
	y *= diff;
}

Float Vector2::Magnitude() const
{
	return sqrtf((x * x) + (y * y));
}

Float Vector2::DotProduct(const Vector2& v) const
{
	return (this->x * v.x) + (this->y * v.y);
}

Float Vector2::GetAngleBetween(const Vector2& v) const
{
	Float tMag = sqrtf((this->x * this->x) + (this->y * this->y));
	Float vMag = sqrtf((v.x * v.x) + (v.y * v.y));

	Float dot = (this->x * v.x) + (this->y * v.y);

	Float a = std::acosf(dot / (tMag * vMag));

	//acosf may return NaN for values close to +- 1.0f, so check for that.
	if (isnan(a)) return 0.0f;
	else return a;
}

Float Vector2::GetProjection(const Vector2& target) const
{
	if (std::fabsf(this->Magnitude()) == 0.0f)
	{
		return 0.0f;
	}

	Float dot = (this->x * target.x) + (this->y * target.y);

	Float tMag = sqrtf((this->x * this->x) + (this->y * this->y));
	Float vMag = sqrtf((target.x * target.x) + (target.y * target.y));

	Float cosA = dot / (tMag * vMag);

	return tMag * cosA;
}

Vector2 Vector2::Perpendicular() const
{
	return Vector2(y, -x);
}

void Vector2::Round()
{
	this->x = std::roundf(x);
	this->y = std::roundf(y);
}

Vector2 Vector2::Rotate(const Vector2& vector, Float angle)
{
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	float newX = vector.x * cosa - vector.y * sina;
	float newY = vector.x * sina + vector.y * cosa;

	return Vector2(newX, newY);
}

Vector2 Vector2::RotateAround(const Vector2& vector, const Vector2& origin, Float angle)
{
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	float xDiff = vector.x - origin.x;
	float yDiff = vector.y - origin.y;

	float newX = cosa * xDiff - sina * yDiff + origin.x;
	float newY = sina * xDiff + cosa * yDiff + origin.y;

	return Vector2(newX, newY);
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
	float mag = sqrtf((vector.x * vector.x) + (vector.y * vector.y));

	return Vector2(vector.x / mag, vector.y / mag);
}

Float Vector2::Magnitude(const Vector2& vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

Float Vector2::DotProduct(const Vector2& a, const Vector2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

Float Vector2::GetAngleBetween(const Vector2& a, const Vector2& b)
{
	Float dot = (a.x * b.x) + (a.y * b.y);

	Float tMag = sqrtf((a.x * a.x) + (a.y * a.y));
	Float vMag = sqrtf((b.x * b.x) + (b.y * b.y));

	Float c = std::acosf(dot / (tMag * vMag));

	//acosf may return NaN for values close to +- 1.0f, so check for that.
	if (isnan(c)) return 0.0f;
	else return c;
}

Float Vector2::GetProjection(const Vector2& vector, const Vector2& target)
{
	Float dot = (vector.x * target.x) + (vector.y * target.y);

	Float tMag = sqrtf((vector.x * vector.x) + (vector.y * vector.y));
	Float vMag = sqrtf((target.x * target.x) + (target.y * target.y));

	if (std::fabsf(tMag) == 0.0f) return 0.0f;

	Float cosA = dot / (tMag * vMag);

	return tMag * cosA;
}

Vector2 Vector2::Perpendicular(const Vector2& v)
{
	return Vector2(v.y, -v.x);
}

Vector2 Vector2::Round(const Vector2& v)
{
	return Vector2(std::roundf(v.x), std::round(v.y));
}

const Vector2 Vector2::UP = Vector2(0.0f, 1.0f);

const Vector2 Vector2::DOWN = Vector2(0.0f, -1.0f);

const Vector2 Vector2::LEFT = Vector2(-1.0f, 0.0f);

const Vector2 Vector2::RIGHT = Vector2(1.0f, 0.0f);

const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);

Vector3::Vector3()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector2& xy)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = 1.0f;
}

Vector3::Vector3(const Vector2& xy, Float z)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
}

Vector3::Vector3(const Vector3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3::Vector3(Vector3&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3& Vector3::operator=(const Vector3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

void Vector3::Set(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::Normalize()
{
	Float mag = std::sqrtf((x * x) + (y * y) + (z * z));

	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
}

void Vector3::Normalize(Float magnitude)
{
	this->x /= magnitude;
	this->y /= magnitude;
	this->z /= magnitude;
}

Float Vector3::Magnitude() const
{
	return std::sqrtf((x * x) + (y * y) + (z * z));
}

void Vector3::Round()
{
	this->x = std::roundf(this->x);
	this->y = std::roundf(this->y);
	this->z = std::roundf(this->z);
}

Float Vector3::AngleBetween(const Vector3& a, const Vector3& b)
{
	Float dot = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	Float magA = std::sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
	Float magB = std::sqrtf((b.x * b.x) + (b.y * b.y) + (b.z * b.z));

	return std::acosf(dot / (magA * magB));
}

Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

Float Vector3::DotProduct(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	Float f = std::sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	return Vector3(
		v.x / f,
		v.y / f,
		v.z / f
	);
}

Float Vector3::Magnitude(const Vector3& v)
{
	return std::sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::DOWN = Vector3(0.0f, -1.0f, 0.0f);

const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);

const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::FORWARD = Vector3(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::BACKWARD = Vector3(0.0f, 0.0f, -1.0f);

const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::IDENTITY = Vector3(0.0f, 0.0f, 1.0f);

Quaternion::Quaternion()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;
}

Quaternion::Quaternion(Float x, Float y, Float z, Float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion Quaternion::AxisAngle(const Vector3& axis, Float angle)
{
	Float sinA = std::sinf(angle / 2.0f);

	return Quaternion(
		axis.x * sinA,
		axis.y * sinA,
		axis.z * sinA,
		std::cosf(angle / 2.0f)
	);
}

Quaternion Quaternion::FromEuler(const Vector3& v)
{
	//Bank = x
	//Heading = y
	//Attitude = z

	Float c1 = std::cosf(v.y / 2.0f);
	Float c2 = std::cosf(v.z / 2.0f);
	Float c3 = std::cosf(v.x / 2.0f);

	Float s1 = std::sinf(v.y / 2.0f);
	Float s2 = std::sinf(v.z / 2.0f);
	Float s3 = std::sinf(v.x / 2.0f);

	return Quaternion(
		s1 * s2 * c3 + c1 * c2 * s3,
		s1 * c2 * c3 + c1 * s2 * s3,
		c1 * s2 * c3 - s1 * c2 * s3,
		c1 * c2 * c3 - s1 * s2 * s3
	);
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return Quaternion(
		-q.x,
		-q.y,
		-q.z,
		q.w
	);
}

Vector3 Quaternion::Rotate(const Vector3& v, const Quaternion& q)
{
	Quaternion p(v.x, v.y, v.z, 0.0f);	//pure quaternion from v
	Quaternion c(-q.x, -q.y, -q.z, q.w); //conjugate quaternion

	Quaternion q1(c * p * q); //multiply quaternions

	return Vector3(q1.x, q1.y, q1.z);	//q1 should be a pure quaternion by this point.
}

Vector3 Quaternion::RotateAround(const Vector3& origin, const Vector3& v, const Quaternion& q)
{
	Vector3 adj(v - origin);

	Quaternion p(adj.x, adj.y, adj.z, 0.0f);
	Quaternion c(-q.x, -q.y, -q.z, q.w);

	Quaternion q1(q * p * c);

	return Vector3(q1.x + origin.x, q1.y + origin.y, q1.y + origin.y);
}

Vector4::Vector4()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Vector4::Vector4(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1.0f;
}

Vector4::Vector4(Float x, Float y, Float z, Float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector2& xy)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = 0.0f;
	this->w = 1.0f;
}

Vector4::Vector4(const Vector2& xy, Float z)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
	this->w = 0.0f;
}

Vector4::Vector4(const Vector2& xy, Float z, Float w)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector3& xyz)
{
	this->x = xyz.x;
	this->y = xyz.y;
	this->z = xyz.z;
	this->w = 1.0f;
}

Vector4::Vector4(const Vector3& xyz, Float w)
{
	this->x = xyz.x;
	this->y = xyz.y;
	this->z = xyz.z;
	this->w = w;
}

Vector4::Vector4(const Vector4& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

Vector4::Vector4(Vector4&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

Vector4& Vector4::operator=(const Vector4& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;

	return *this;
}

const Vector4 Vector4::UP =			Vector4(0.0f, 1.0f, 0.0f);
const Vector4 Vector4::DOWN =		Vector4(0.0f, -1.0f, 0.0f);
const Vector4 Vector4::LEFT =		Vector4(-1.0f, 0.0f, 0.0f);
const Vector4 Vector4::RIGHT =		Vector4(1.0f, 0.0f, 0.0f);
const Vector4 Vector4::FORWARD =	Vector4(0.0f, 0.0f, 1.0f);
const Vector4 Vector4::BACKWARD =	Vector4(0.0f, 0.0f, -1.0f);
const Vector4 Vector4::ONE =		Vector4(1.0f, 1.0f, 1.0f);
const Vector4 Vector4::IDENTITY =	Vector4(0.0f, 0.0f, 0.0f);