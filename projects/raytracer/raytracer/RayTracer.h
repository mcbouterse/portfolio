#ifndef MB_INC_RAYTRACER_H
#define MB_INC_RAYTRACER_H

#include <vector>
#include <string>
#include "ColorRGB.h"


struct RayTraceSettings
{
	int      resolutionWidth;
	int      resolutionHeight;
	float    depth;
	ColorRGB clearColor;
};


class RayTracer
{
public:
	RayTracer() { }
	RayTracer(const RayTraceSettings& p_settings)
	:
	m_settings(p_settings)
	{ }

	bool renderScene();

	bool saveToFile(const std::string& p_filename);


private:
	RayTraceSettings m_settings;
	std::vector<ColorRGB> m_imageBuffer;
};


#endif // MB_INC_RAYTRACER_H
