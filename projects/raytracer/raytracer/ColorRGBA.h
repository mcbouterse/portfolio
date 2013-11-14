#ifndef MB_INC_COLORRGBA_H
#define MB_INC_COLORRGBA_H

#include "types.h"
#include "ColorRGB.h"


class ColorRGBA
{
public:
	ColorRGBA()
	:
	r(255),
	g(255),
	b(255),
	a(255)
	{}

	ColorRGBA(u8 p_r, u8 p_g, u8 p_b, u8 p_a)
	:
	r(p_r),
	g(p_g),
	b(p_b),
	a(p_a)
	{}

	inline ColorRGB rgb() { return ColorRGB(r, g, b); }

	u8 r;
	u8 g;
	u8 b;
	u8 a;
};


#endif // MB_INC_COLORRGBA_H
