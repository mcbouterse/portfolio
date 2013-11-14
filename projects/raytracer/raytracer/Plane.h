#ifndef MB_INC_PLANE_H
#define MB_INC_PLANE_H

#include "ColorRGBA.h"
#include "Ray.h"
#include "Shape.h"
#include "Vector3.h"
#include "Point3.h"


class Plane : public Shape
{
public:
	Plane()
	:
	m_normal(0, 1, 0),
	m_distance(0)
	{ }

	Plane(const Vector3& p_normal, const Point3& p_point, const ColorRGBA& p_color)
	:
	Shape     (p_color),
	m_normal  (p_normal),
	m_distance(-dotProduct(p_point, p_normal))
	{ }

	float getFirstIntersection(const Ray& p_ray);

private:
	Vector3 m_normal;
	float   m_distance;
};


#endif // MB_INC_PLANE_H
