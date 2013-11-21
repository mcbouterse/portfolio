#include <iostream>
#include <numeric>
#include <SDL.h>

#include <RayTracer.h>
#include <Sphere.h>
#include <Plane.h>


Sphere* spheres[4] = {};


void buildScene(SceneList& p_scene)
{
	spheres[0] = new Sphere(Point3(-320, 0, 50), 200.0f, ColorRGBA(255, 0, 255, 255));
	spheres[1] = new Sphere(Point3(320, 0, 50), 200.0f, ColorRGBA(0, 255, 255, 255));
	spheres[2] = new Sphere(Point3(0, 0, -50), 200.0f, ColorRGBA(255, 255, 0, 255));
	//spheres[3] = new Sphere(Point3(0, 500, 0), 10.0f, ColorRGBA(255, 255, 255, 255));

	p_scene.push_back(spheres[0]);
	p_scene.push_back(spheres[1]);
	p_scene.push_back(spheres[2]);
	//p_scene.push_back(spheres[3]);

	//p_scene.push_back(new Plane(Vector3(0, 1, 0), Vector3(0, 750, 0), ColorRGBA(255, 0, 0, 255)));
	//p_scene.back()->reflective = false;

	p_scene.push_back(new Plane(Vector3(0, 1, 0), Vector3(0, -250, 0), ColorRGBA(255, 255, 255, 255)));
	//p_scene.back()->reflective = true;
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


void showFrameStats(const std::vector<Uint64>& p_timings)
{
	Uint64 totalTicks(0);
	totalTicks = std::accumulate(p_timings.begin(), p_timings.end(), totalTicks);
	
	float secondsPassed = static_cast<float>(totalTicks) / SDL_GetPerformanceFrequency();
	float averageTime = secondsPassed / p_timings.size();
	std::cout << "Total render time " << secondsPassed << " seconds." << std::endl;
	std::cout << "Average frame render time " << averageTime * 1000.0f << " ms, " << 1.0f / averageTime << " fps." << std::endl;
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
	settings.clearColor = ColorRGB(0, 0, 50);
	settings.eyePosition = Point3(0, 0, -1024);
	settings.lightEnabled = false;

	RayTracer rayTracer(settings);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* backbuffer = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB24,
		SDL_TEXTUREACCESS_STREAMING,
		settings.resolutionWidth,
		settings.resolutionHeight);

	std::vector<Uint64> frameTimes;
	frameTimes.reserve(36000);

	// Main application loop
	bool keepRunning(true);
	bool lightingEnabled(false);
	
	while (keepRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) keepRunning = false;
			if (event.type == SDL_KEYDOWN)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_0:
					lightingEnabled = !lightingEnabled;
					rayTracer.setLightingEnabled(lightingEnabled);
					break;
				case SDLK_9:
					scene.back()->reflective = !scene.back()->reflective;
				default:
					break;
				}
			}
		}
		
		// Animate scene objects
		updateScene();
		
		Uint64 startRender = SDL_GetPerformanceCounter();
		rayTracer.renderScene(scene);
		frameTimes.push_back(SDL_GetPerformanceCounter() - startRender);
		
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
	
	showFrameStats(frameTimes);
	
	SDL_DestroyTexture(backbuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
