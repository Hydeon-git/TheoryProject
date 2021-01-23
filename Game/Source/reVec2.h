#pragma once

#include <math.h>

class reVec2
{
public:
	double x = 0;
	double y = 0;

public:
	reVec2() {};

	reVec2(float newX, float newY)
	{
		x = newX;
		y = newY;
	}

	// Set new values for the vector
	void SetVector(float newX, float newY)
	{
		x = newX;
		y = newY;
	}

	// Ngeate a vector
	reVec2 Negate(reVec2 v)
	{
		v.x = -v.x;
		v.y = -v.y;
	}

	// Negate a vector
	reVec2 Negate()
	{
		reVec2 aux;
		aux.x = -x;
		aux.y = -y;

		return aux;
	}

	// Get the perpendicular vector
	reVec2 Perpendicular(reVec2& v)
	{
		reVec2 ret;
		ret.x = v.y;
		ret.y = -v.x;
		return ret;
	}

	reVec2 Normalize()
	{
		reVec2 aux = reVec2(x, y);
		float l = sqrt(pow(aux.x, 2) + pow(aux.y, 2));
		aux = aux / l;
		return aux;
	}

	float Dot(reVec2& v)
	{
		return x * v.x + y * v.y;
	}

	// Vector addition
	void operator += (const reVec2& v)
	{
		x += v.x;
		y += v.y;
	}

	reVec2 operator + (const reVec2& v)
	{
		reVec2 nv;
		nv.x = x + v.x;
		nv.y = y + v.y;
		return nv;
	}

	reVec2 operator + (const float& a)
	{
		reVec2 nv;
		nv.x = x + a;
		nv.y = y + a;
		return nv;
	}

	// Vector substraction
	void operator -= (const reVec2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	reVec2 operator- (reVec2& v)
	{
		reVec2 nv;
		nv.x = x - v.x;
		nv.y = y - v.y;
		return nv;
	}

	// Multply a vector by a scalar
	reVec2 operator *= (float a)
	{
		reVec2 nv;
		nv.x = x * a;
		nv.y = y * a;
		return nv;
	}

	reVec2 operator*(reVec2& a)
	{
		reVec2 nv;
		nv.x = x * a.x;
		nv.y = y * a.y;
		return nv;
	}

	reVec2 operator*(float a)
	{
		reVec2 aux;
		aux.x = x * a;
		aux.y = y * a;

		return aux;
	}

	void operator*(double& a)
	{
		x = x * a;
		y = y * a;
	}

	reVec2 operator/(float& a)
	{
		reVec2 nv;
		nv.x = x / a;
		nv.y = y / a;
		return nv;
	}

	bool operator< (reVec2 v)
	{
		return (x < v.x) && (y < v.y);
	}
};