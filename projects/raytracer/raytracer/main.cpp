#include <algorithm>
#include <iostream>
#include "Plane.h"
#include "RayTracer.h"
#include "Sphere.h"


struct deleteDynamicObject
{
	template<typename T>
	void operator()(const T* ptr) const
	{
		if (ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
};



int main()
{
	// Build scene
	SceneList scene;
	
	scene.push_back(new Sphere(Point3(-320,0,50), 200.0f, ColorRGBA(255,0,0,255)));
	scene.push_back(new Sphere(Point3(320, 0, 50), 200.0f, ColorRGBA(0, 0, 255, 255)));
	scene.push_back(new Sphere(Point3(0, 0, -50), 200.0f, ColorRGBA(0, 255, 0, 255)));
	scene.push_back(new Plane(Vector3(0,1,0), Vector3(0, -150, 0), ColorRGBA(255,255,255,255)));
	
	// Render the scene
	RayTraceSettings settings;
	settings.resolutionWidth  = 600;
	settings.resolutionHeight = 480;
	settings.depth = 255;
	settings.clearColor = ColorRGB(50,50,50);
	settings.eyePosition = Point3(0, 0, 600);
	
	RayTracer rayTracer(settings);
	rayTracer.renderScene(scene);
	
	rayTracer.saveToFile("rayTracerTest.tga");

	// Clean up the scene
	std::for_each(scene.begin(), scene.end(), deleteDynamicObject());
	
	return 0;
}
