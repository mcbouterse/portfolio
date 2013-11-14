#ifndef MB_INC_SPHERE_H
#define MB_INC_SPHERE_H

#include "ColorRGBA.h"
#include "Ray.h"
#include "Shape.h"
#include "Vector3.h"
#include "Point3.h"


class Sphere : public Shape
{
public:
	Sphere()
	:
	m_center(0, 0, 0),
	m_radius(1)
	{ }

	Sphere(const Point3& p_center, float p_radius, const ColorRGBA& p_color)
	:
	Shape(p_color),
	m_center(p_center),
	m_radius(p_radius)
	{ }

	float getFirstIntersection(const Ray& p_ray);

private:
	Point3 m_center;
	float  m_radius;
};


#endif // MB_INC_SPHERE_H
