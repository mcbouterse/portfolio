#ifndef MB_INC_VECTOR3_H
#define MB_INC_VECTOR3_H

#include <cmath>


class Vector3
{
public:
	Vector3()
	:
	x(0.0f),
	y(0.0f),
	z(0.0f)
	{ }

	Vector3(float p_x, float p_y, float p_z)
	:
	x(p_x),
	y(p_y),
	z(p_z)
	{ }

	inline Vector3& operator-=(const Vector3& p_rhs)
	{
		x -= p_rhs.x;
		y -= p_rhs.y;
		z -= p_rhs.z;
		return *this;
	}

	inline Vector3& operator+=(const Vector3& p_rhs)
	{
		x += p_rhs.x;
		y += p_rhs.y;
		z += p_rhs.z;
		return *this;
	}

	inline void normalize()
	{
		const float length = sqrt(x*x+y*y+z*z);
		x /= length;
		y /= length;
		z /= length;
	}

	float x;
	float y;
	float z;
};


inline Vector3 operator+(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return Vector3(p_lhs.x + p_rhs.x, p_lhs.y + p_rhs.y, p_lhs.z + p_rhs.z);
}


inline Vector3 operator-(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return Vector3(p_lhs.x - p_rhs.x, p_lhs.y - p_rhs.y, p_lhs.z - p_rhs.z);
}

inline Vector3 operator*(float p_lhs, const Vector3& p_rhs)
{
	return Vector3(p_lhs * p_rhs.x, p_lhs * p_rhs.y, p_lhs * p_rhs.z);
}


inline float dotProduct(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return p_lhs.x * p_rhs.x + p_lhs.y * p_rhs.y + p_lhs.z * p_rhs.z;
}


inline Vector3 reflect(const Vector3& p_incident, const Vector3& p_normal)
{
	return p_incident - (2 * dotProduct(p_incident, p_normal) * p_normal);
}



#endif // MB_INC_VECTOR3_H
