#ifndef MB_INC_COLORRGB_H
#define MB_INC_COLORRGB_H

#include "types.h"
#include "Vector3.h"


class ColorRGB
{
public:
	ColorRGB()
	:
	r(255),
	g(255),
	b(255)
	{}

	ColorRGB(u8 p_r, u8 p_g, u8 p_b)
	:
	r(p_r),
	g(p_g),
	b(p_b)
	{}

	inline ColorRGB& operator+=(const ColorRGB& p_rhs)
	{
		r += p_rhs.r;
		g += p_rhs.g;
		b += p_rhs.b;
		return *this;
	}

	inline Vector3 normalized()
	{
		return Vector3(r/255.0f, g/255.0f,b/255.0f);
	}

	u8 r;
	u8 g;
	u8 b;
};

inline ColorRGB operator*(float p_lhs, const ColorRGB& p_rhs)
{
	return ColorRGB(
		static_cast<u8>(p_lhs * p_rhs.r),
		static_cast<u8>(p_lhs * p_rhs.g),
		static_cast<u8>(p_lhs * p_rhs.b));
}


#endif // MB_INC_COLORRGBA_H
