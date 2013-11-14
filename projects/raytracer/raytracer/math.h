#ifndef MB_INC_MATH_H
#define MB_INC_MATH_H

static const float epsilon = 0.00001f;

inline bool floatEqual(float p_lhs, float p_rhs)
{
	float difference = p_lhs - p_rhs;
	return difference < epsilon && difference > -epsilon;
}



#endif // MB_INC_MATH_H
