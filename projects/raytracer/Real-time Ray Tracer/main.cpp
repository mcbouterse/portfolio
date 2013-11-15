#include <iostream>
#include <SDL.h>

#include <RayTracer.h>
#include <Sphere.h>
#include <Plane.h>


Sphere* spheres[3];


void buildScene(SceneList& p_scene)
{
	spheres[0] = new Sphere(Point3(-320, 0, 50), 200.0f, ColorRGBA(255, 0, 0, 255));
	spheres[1] = new Sphere(Point3(320, 0, 50), 200.0f, ColorRGBA(0, 0, 255, 255));
	spheres[2] = new Sphere(Point3(0, 0, -50), 200.0f, ColorRGBA(0, 255, 0, 255));

	p_scene.push_back(spheres[0]);
	p_scene.push_back(spheres[1]);
	p_scene.push_back(spheres[2]);
	p_scene.push_back(new Plane(Vector3(0, 1, 0), Vector3(0, -150, 0), ColorRGBA(255, 255, 255, 255)));
}


void updateScene()
{
	const float timeInMs = static_cast<float>(SDL_GetTicks());

	float t0 = timeInMs * 0.001f;
	float t1 = t0 + 0.6667f * static_cast<float>(M_PI);
	float t2 = t1 + 0.6667f * static_cast<float>(M_PI);
	const float radius = 350.0f;
	spheres[0]->setPosition(Point3(sin(t0) * radius, 0, cos(t0) * radius));
	spheres[1]->setPosition(Point3(sin(t1) * radius, 0, cos(t1) * radius));
	spheres[2]->setPosition(Point3(sin(t2) * radius, 0, cos(t2) * radius));
}


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Real-Time Ray Tracing Experiment", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Build scene
	SceneList scene;
	buildScene(scene);

	// Create the RayTracer
	RayTraceSettings settings;
	settings.resolutionWidth  = 400;
	settings.resolutionHeight = 300;
	settings.depth = 255;
	settings.clearColor = ColorRGB(50, 50, 50);
	settings.eyePosition = Point3(0, 0, -1024);

	RayTracer rayTracer(settings);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* backbuffer = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB24,
		SDL_TEXTUREACCESS_STREAMING,
		settings.resolutionWidth,
		settings.resolutionHeight);

	// Main application loop
	bool keepRunning(true);
	while (keepRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)	keepRunning = false;
		}

		// Animate scene objects
		updateScene();

		Uint64 startRender = SDL_GetPerformanceCounter();
		rayTracer.renderScene(scene);
		Uint64 stopRender = SDL_GetPerformanceCounter();

		float secondsPassed = static_cast<float>(stopRender - startRender) / SDL_GetPerformanceFrequency();
		std::cout << "Render took " << secondsPassed << " seconds." << std::endl;

		// Update the texture with raytraced result
		u8* sourceData = rayTracer.getPixelData();
		int sourcePitch = settings.resolutionWidth * 3;
		u8* textureData(nullptr);
		int pitch(0);
		SDL_LockTexture(backbuffer, nullptr, reinterpret_cast<void**>(&textureData), &pitch);

		for (int row = 0; row < settings.resolutionHeight; row++)
		{
			u8* sourceRow = &sourceData[(settings.resolutionHeight - row - 1) * sourcePitch];
			memcpy(textureData, sourceRow, sourcePitch);
			textureData += pitch;
		}

		SDL_UnlockTexture(backbuffer);

		// TODO: Show result on screen!
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backbuffer, nullptr, nullptr);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(backbuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
