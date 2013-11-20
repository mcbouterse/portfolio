#include "Sphere.h"

#include <cmath>
#include "math_helpers.h"


float Sphere::getFirstIntersection(const Ray& p_ray)
{
	Vector3 rayToSphere = m_center - p_ray.origin;

	// NOTE: Actually length squared
	float rayToSphereLength = dotProduct(rayToSphere, rayToSphere);
	
	float intersectA = dotProduct(rayToSphere, p_ray.direction);
	
	if (intersectA < 0.0f)
	{
		return -1.0f;
	}

	float intersectB = (m_radius * m_radius) - rayToSphereLength + (intersectA * intersectA);

	if (intersectB < 0.0f)
	{
		return -1.0f;
	}
	
	// The ray hit the plane
	return intersectA - sqrt(intersectB);
}


Vector3 Sphere::getNormal(const Point3& p_point)
{
	return p_point - m_center;
}

