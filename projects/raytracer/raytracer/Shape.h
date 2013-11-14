#ifndef MB_INC_SHAPE_H
#define MB_INC_SHAPE_H


#include "ColorRGBA.h"
#include "Ray.h"


class Shape
{
public:
	Shape() { }
	Shape(const ColorRGBA& p_color)
	:
	color(p_color)
	{ }

	virtual float getFirstIntersection(const Ray& p_ray) = 0;

	ColorRGBA color;
};


#endif // MB_INC_SHAPE_H
