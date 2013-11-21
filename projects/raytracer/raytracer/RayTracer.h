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
	int      maxBounces;
	bool     lightEnabled;

	RayTraceSettings()
	:
	resolutionWidth(640),
	resolutionHeight(480),
	depth(255),
	clearColor(0,0,0),
	eyePosition(0,0,-100),
	maxBounces(5),
	lightEnabled(true)
	{ }
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
	
	void setLightingEnabled(bool p_enable) { m_settings.lightEnabled = p_enable; }
	
private:
	ColorRGB trace(const Ray& p_ray, const SceneList& p_scene, int p_bounce);
	RayTraceSettings m_settings;
	std::vector<ColorRGB> m_imageBuffer;
};


#endif // MB_INC_RAYTRACER_H
