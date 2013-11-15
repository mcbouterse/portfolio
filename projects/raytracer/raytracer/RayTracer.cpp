#include "RayTracer.h"

#include "targa.h"
#include "Ray.h"


void RayTracer::renderScene(const SceneList& p_scene)
{
	if (m_imageBuffer.empty())
	{
		m_imageBuffer.resize(m_settings.resolutionWidth * m_settings.resolutionHeight);
	}

	int pixelIndex(0);
	Ray primaryRay(m_settings.eyePosition, Vector3(0, 0, 1));

	const float halfWidth  = m_settings.resolutionWidth  / 2.0f;
	const float halfHeight = m_settings.resolutionHeight / 2.0f;
		
	for (int y = 0; y < m_settings.resolutionHeight; ++y)
	{
		for (int x = 0; x < m_settings.resolutionWidth; ++x)
		{
			// Compute direction of ray based on pixel position
			primaryRay.direction = Vector3(x - halfWidth, y - halfHeight, m_settings.depth);
			primaryRay.direction.normalize();
				
			m_imageBuffer[pixelIndex] = trace(primaryRay, p_scene);
			++pixelIndex;
		}

	}
}


bool RayTracer::saveToFile(const std::string& p_filename)
{
	return writeTGAFile(p_filename, m_settings.resolutionWidth, m_settings.resolutionHeight, reinterpret_cast<u8*>(&m_imageBuffer[0]));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Private

ColorRGB RayTracer::trace(const Ray& p_ray, const SceneList& p_scene)
{
	float closestDistance = std::numeric_limits<float>::max();
	Shape* closestShape(nullptr);

	for (auto it = p_scene.begin(); it != p_scene.end(); ++it)
	{
		if (*it == nullptr) continue;

		float intersectionDistance = (*it)->getFirstIntersection(p_ray);
		
		if (intersectionDistance > 0.0f && intersectionDistance < closestDistance)
		{
			closestDistance = intersectionDistance;
			closestShape = *it;
		}
	}

	if (closestShape != nullptr)
	{
		return closestShape->color.rgb();
	}
	return m_settings.clearColor;
}
