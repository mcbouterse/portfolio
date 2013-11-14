#ifndef MB_INC_RAY_H
#define MB_INC_RAY_H

#include "Vector3.h"
#include "Point3.h"


class Ray
{
public:
	Ray();
	Ray(const Point3& p_origin, const Vector3& p_direction)
	:
	origin   (p_origin),
	direction(p_direction)
	{ }

	~Ray() { }

	Point3  origin;
	Vector3 direction;
};


#endif // MB_INC_RAY_H
