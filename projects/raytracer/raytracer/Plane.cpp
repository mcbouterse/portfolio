#include "Plane.h"

#include "math.h"


float Plane::getFirstIntersection(const Ray& p_ray)
{
	float rayPlaneAngle = dotProduct(m_normal, p_ray.direction);
	
	// Check if ray is parallel to the plane
	if (floatEqual(rayPlaneAngle, 0.0f))
	{
		return -1.0f;
	}
	
	// TODO: Check for back side of plane?
	
	float originDistance = -(dotProduct(m_normal, p_ray.origin) + m_distance);
	float intersection = originDistance / rayPlaneAngle;
	
	// Check if ray is facing away from the plane
	if (intersection < 0.0f)
	{
		return -1.0f;
	}
	
	// The ray hit the plane
	return intersection;
}

