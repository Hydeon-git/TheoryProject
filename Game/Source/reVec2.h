#pragma once

#include <math.h>

class reVec2
{
public:
	reVec2() {};

	reVec2(float _x, float _y)
	{
		x = _x;
		y = _y;
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
	float Normalize()
	{
		float length = Length();
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
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

	float x, y;
};