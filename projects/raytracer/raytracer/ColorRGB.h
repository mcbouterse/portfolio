#ifndef MB_INC_COLORRGB_H
#define MB_INC_COLORRGB_H


class ColorRGB
{
public:
	ColorRGB()
	:
	r(255),
	g(255),
	b(255)
	{}

	ColorRGB(unsigned char p_r, unsigned char p_g, unsigned char p_b)
	:
	r(p_r),
	g(p_g),
	b(p_b)
	{}

	unsigned char r;
	unsigned char g;
	unsigned char b;
};


#endif // MB_INC_COLORRGBA_H
