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

	// Set this vector to some specified coordinates.
	void Set(float x_, float y_) { x = x_; y = y_; }

	// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	// Add a vector
	void operator +=(const reVec2& v)
	{
		x += v.x;
		y += v.y;
	}
	reVec2 operator +(const reVec2& v)
	{
		reVec2 vec;
		vec.x = x + v.x;
		vec.y = y + v.y;
		return vec;
	}

	// Subtract a vector
	void operator -=(const reVec2& v)
	{
		x -= v.x;
		y -= v.y;
	}
	reVec2 operator -(const reVec2& v)
	{
		reVec2 vec;
		vec.x = x - v.x;
		vec.y = y - v.y;
		return vec;
	}

	// Multply a vector
	void operator *=(float a)
	{
		x *= a;
		y *= a;
	}
	reVec2 operator *(float a)
	{
		reVec2 v;
		v.x = x * a;
		v.y = y * a;
		return v;
	}

	// Divide a vector
	void operator /=(float& a)
	{
		x /= a;
		y /= a;
	}
	reVec2 operator /(float& a)
	{
		reVec2 v;
		v.x = x / a;
		v.y = y / a;
		return v;
	}

	//Compare vectors
	bool operator< (reVec2 v)
	{
		return (x < v.x) && (y < v.y);
	}
	bool operator== (reVec2 v)
	{
		return (x == v.x) && (y == v.y);
	}

	/// Convert this vector into a unit vector. Returns the length.
	reVec2 Normalize()
	{
		reVec2 aux = reVec2(x, y);
		float l = sqrt(pow(aux.x, 2) + pow(aux.y, 2));
		if (l != 0)
		{
			aux = aux / l;
			return aux;
		}
		else return { 0,0 };
	}

	// Length of vector (norm)
	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	// Length squared
	float LengthSquared() const
	{
		return x * x + y * y;
	}

};