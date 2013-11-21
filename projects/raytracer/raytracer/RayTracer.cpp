#include "RayTracer.h"

#include <algorithm>
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
				
			m_imageBuffer[pixelIndex] = trace(primaryRay, p_scene, 0);
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

std::pair<Shape*, float> getClosestShape(const Ray& p_ray, const SceneList& p_scene)
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
	return std::make_pair(closestShape, closestDistance);
}

ColorRGB RayTracer::trace(const Ray& p_ray, const SceneList& p_scene, int p_bounce)
{
	if (p_bounce > m_settings.maxBounces)
	{
		return m_settings.clearColor;
	}
	
	std::pair<Shape*, float> closestInfo = getClosestShape(p_ray, p_scene);

	if (closestInfo.first != nullptr)
	{
		Point3 intersectionPoint = p_ray.origin + closestInfo.second * p_ray.direction;
		Vector3 intersectionNormal = closestInfo.first->getNormal(intersectionPoint);
		intersectionNormal.normalize();
		
		// Offset to prevent self intersection
		intersectionPoint += 0.0001f * intersectionNormal;
		
		//ColorRGB reflectedColor = ColorRGB(255, 255, 255);
		
		if (closestInfo.first->reflective)
		{
			// Trace reflection ray
			Ray reflectionRay(
				intersectionPoint,
				reflect(p_ray.direction, intersectionNormal));
			
			return trace(reflectionRay, p_scene, p_bounce + 1);
		}
		if (m_settings.lightEnabled == false)
		{
			return closestInfo.first->color.rgb();
		}
		
		// Compute shadow ray 
		const Point3 lightPosition(0, 500, 0);
		
		Vector3 shadowVector = lightPosition - intersectionPoint;
		shadowVector.normalize();
		Ray shadowRay(intersectionPoint, shadowVector);
		
		std::pair<Shape*, float> shadowInfo = getClosestShape(shadowRay, p_scene);
		
		float lightContribution = 0.2f; // Ambient light
		
		if (shadowInfo.first == nullptr)
		{
			// Compute diffuse light factor
			float diffuseFactor = dotProduct(intersectionNormal, shadowRay.direction);
			if (diffuseFactor > 0.0f)
			{
				lightContribution += diffuseFactor;
			}
		}
		return std::min(lightContribution, 1.0f) * closestInfo.first->color.rgb();
	}
	return m_settings.clearColor;
}
