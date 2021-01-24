#pragma once

#include "reVec2.h"
#include "SString.h"
#include "Globals.h"

enum class reBodyType
{
	EARTH_GRAVITY,
	MOON_GRAVITY,
	NO_GRAVITY,
};

class reBody
{
public:
	reBodyType type;

private:
	reVec2 position;
	float angle;

	reVec2 linearVelocity;

	reVec2 acceleration;
	reVec2 force;
	float mass;

	bool active;

public:
	reBody(reBodyType t)
	{
		position.Set(0.0f, 0.0f);
		angle = 0.0f;
		linearVelocity.Set(0.0f, 0.0f);
		acceleration.Set(0.0f, 0.0f);
		force.Set(0.0f, 0.0f);
		mass = 0.0f;
		type = t;
		active = true;
	}

	~reBody() {};

	void AddForce(reVec2 f)
	{
		force += f;
		acceleration = force / mass;
	}

	void AddMomentum(float a, float dt)
	{
		reVec2 momentum = reVec2(cos(a - PI / 2), sin(a - PI / 2));
		reVec2 v = reVec2(PIXEL_TO_METERS(momentum.x * 250 * dt), PIXEL_TO_METERS(momentum.y * 250 * dt));
		linearVelocity += v;
	}

	void AddNegativeMomentum(float a, float dt)
	{
		reVec2 momentum = reVec2(cos(a - PI / 2), sin(a - PI / 2));
		reVec2 v = reVec2(PIXEL_TO_METERS(momentum.x * 250 * dt), PIXEL_TO_METERS(momentum.y * 250 * dt));
		linearVelocity -= v;
	}

	// Rotate a body. NOTE: ang must be on DEGREES.
	void Rotate(float ang)
	{
		angle += ang * PI / 180;
		if (angle >= 360) angle = 0;
	}

	// Set the position of the body
	inline void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	// Get the position of the body
	inline reVec2& GetPosition()
	{
		return position;
	}

	//Set the angle of the body (in radians)
	inline void SetAngle(float a)
	{
		angle = a;
	}

	// Get the angle of the body (in radians)
	inline float& GetAngle()
	{
		return angle;
	}

	// Set the linear velocity of the body 
	inline void SetLinearVelocity(float x, float y)
	{
		linearVelocity.x = x;
		linearVelocity.y = y;
	}

	// Get the linear velocity of the body 
	inline reVec2& GetLinearVelocity()
	{
		return linearVelocity;
	}

	// Set the acceleration of the body 
	inline void SetAcceleration(float x, float y)
	{
		acceleration.x = x;
		acceleration.y = y;
	}

	// Get the acceleration of the body 
	inline reVec2& GetAcceleration()
	{
		return acceleration;
	}

	// Set the mass of the body
	inline void SetMass(float m)
	{
		mass = m;
	}

	// Get the mass of the body
	inline float& GetMass()
	{
		return mass;
	}

	// Set the type of the body
	inline void SetType(reBodyType t)
	{
		type = t;
	}

	// Get the type of the body
	inline reBodyType GetType()
	{
		return type;
	}

	// Set the active state of the body
	inline void SetActive(bool act)
	{
		active = act;
	}

	// Get the active state of the body
	inline bool IsActive()
	{
		return active;
	}

};