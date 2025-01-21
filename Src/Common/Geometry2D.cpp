//#include <Dxlib.h>
#include <cmath>
#include <algorithm>
#include "Geometry2D.h"

//Vector2 operator-(const Vector2& va, const Vector2 vb)
//{
//	return Vector2(va.x - vb.x, va.y - vb.y);
//}
//
//Vector2 operator+(const Vector2& va, const Vector2 vb)
//{
//	return Vector2(va.x + vb.x, va.y + vb.y);
//}


Vector2 ToVector2(const Vector2f& val)
{
	return Vector2(val.x, val.y);
}

Vector2f ToVector2f(const Vector2& val)
{
	return Vector2f(val.x, val.y);
}

float Differential(float x, float h)
{
	return (Squared(x + h) - Squared(x)) / h;
}

float Pow(float val, float p)
{
	return powf(val,p);
}

float Squared(float val)
{
	return powf(val,2.0f);
}
