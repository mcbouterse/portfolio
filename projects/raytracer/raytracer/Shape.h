#ifndef MB_INC_SHAPE_H
#define MB_INC_SHAPE_H


#include "ColorRGBA.h"


class Shape
{
public:
	Shape() { }
	Shape(const ColorRGBA& p_color)
	:
	color(p_color)
	{ }

	ColorRGBA color;
};


#endif // MB_INC_SHAPE_H
