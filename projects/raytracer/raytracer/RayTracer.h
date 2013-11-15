#ifndef MB_INC_RAYTRACER_H
#define MB_INC_RAYTRACER_H

#include <vector>
#include <string>
#include "ColorRGB.h"
#include "Point3.h"
#include "Shape.h"
#include "types.h"


struct RayTraceSettings
{
	int      resolutionWidth;
	int      resolutionHeight;
	float    depth;
	ColorRGB clearColor;
	Point3   eyePosition;
};

typedef std::vector<Shape*> SceneList;


class RayTracer
{
public:
	RayTracer() { }
	RayTracer(const RayTraceSettings& p_settings)
	:
	m_settings(p_settings)
	{ }
	
	void renderScene(const SceneList& p_scene);
	
	bool saveToFile(const std::string& p_filename);

	u8* getPixelData() { return reinterpret_cast<u8*>(&m_imageBuffer[0]); }
	
	
private:
	ColorRGB trace(const Ray& p_ray, const SceneList& p_scene);
	RayTraceSettings m_settings;
	std::vector<ColorRGB> m_imageBuffer;
};


#endif // MB_INC_RAYTRACER_H
