#ifndef MB_INC_VECTOR3_H
#define MB_INC_VECTOR3_H


class Vector3
{
public:
	Vector3();
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

	float x;
	float y;
	float z;
};


inline float dotProduct(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return p_lhs.x * p_rhs.x + p_lhs.y * p_rhs.y + p_lhs.z * p_rhs.z;
}


inline Vector3 operator+(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return Vector3(p_lhs.x + p_rhs.x, p_lhs.y + p_rhs.y, p_lhs.z + p_rhs.z);
}


inline Vector3 operator-(const Vector3& p_lhs, const Vector3& p_rhs)
{
	return Vector3(p_lhs.x - p_rhs.x, p_lhs.y - p_rhs.y, p_lhs.z - p_rhs.z);
}


#endif // MB_INC_VECTOR3_H
