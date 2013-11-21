#ifndef MB_INC_SHAPE_H
#define MB_INC_SHAPE_H


#include "ColorRGBA.h"
#include "Ray.h"


class Shape
{
public:
	Shape() { }
	Shape(const ColorRGBA& p_color, bool p_reflective = false)
	:
	color(p_color),
	reflective(p_reflective)
	{ }

	virtual float getFirstIntersection(const Ray& p_ray) = 0;
	virtual Vector3 getNormal(const Point3&) = 0;

	ColorRGBA color;
	bool      reflective;
};


#endif // MB_INC_SHAPE_H
