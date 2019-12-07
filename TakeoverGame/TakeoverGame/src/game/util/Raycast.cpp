#include "Raycast.h"

using namespace tkv;

Ray::Ray(const Vector3& direction, const Vector3& location)
{
	this->direction = Normalize(direction);
	this->location = location;
}

CircleVolume::CircleVolume(const Vector3& location, Float radius)
{
	this->location = location;
	this->radius = radius;
}

Bool CircleVolume::Intersects(const Ray& r)
{
	//Get X/Z location of ray at y-coord

	Vector2 intersection(
		(-r.direction.x / r.direction.y) * (r.location.y - this->location.y) + r.location.x,
		(-r.direction.z / r.direction.y) * (r.location.y - this->location.y) + r.location.z
	);

	Vector2 planePos(
		this->location.x,
		this->location.z
	);

	return (Distance(intersection, planePos) <= radius);
}