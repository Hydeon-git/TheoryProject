#pragma once

#include "reVec2.h"
#include "SString.h"
#include "Globals.h"

enum class BodyType
{
	DYNAMIC,
	STATIC,
	NO_GRAVITY,
};

class reBody
{
private:
	reVec2 position;
	reVec2 linearV;
	reVec2 maxLinearV;
	float radius;
	float angularV;
	reVec2 acceleration;
	reVec2 force;
	float mass;
	float bodyGravity;
	float angle;
	SString name;
	bool active;

public:
	BodyType type;

public:
	// Default constructor that sets all the variables to default
	reBody(SString n, BodyType t, bool act = true)
	{
		name = n;
		type = t;
		active = act;
		position = reVec2(0.0f, 0.0f);
		linearV = reVec2(0.0f, 0.0f);
		maxLinearV = reVec2(0.0f, 0.0f);
		//angularV = reVec2(0.0f, 0.0f);
		acceleration = reVec2(0.0f, 0.0f);
		force = reVec2(0.0f, 0.0f);
		mass = 0.0f;
		bodyGravity = 0.0f;
		angle = 0.0f;
	}

	virtual ~reBody() {};

	void AddForce(reVec2 f)
	{
		force += f;
		acceleration = force / mass;
	}

	void AddMomentum(float a, float dt)
	{
		reVec2 momentum = reVec2(cos(a - PI / 2), sin(a - PI / 2));
		reVec2 v = reVec2(PIXEL_TO_METERS(momentum.x * 250 * dt), PIXEL_TO_METERS(momentum.y * 250 * dt));
		linearV += v;
	}

	// Rotate a body. NOTE: ang must be on DEGREES.
	void Rotate(float ang)
	{
		angle += ang * PI / 180;
		if (angle >= 360) angle = 0;
	}

	bool IsActive() { return active; }

	// ===================================================
	//					Setters
	// ===================================================

	void SetActive(bool act)
	{
		active = act;
	}

	void SetMass(float m)
	{
		mass = m;
	}

	// Set body position 
	void SetPosition(reVec2 pos)
	{
		position = pos;
	}
	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	// Set body linear speed
	void SetLinearVelocity(reVec2 v)
	{
		linearV = v;
	}

	void SetLinearVelocity(float x, float y)
	{
		linearV.x = x;
		linearV.y = y;
	}

	void SetMaxLinearVelocity(reVec2 v)
	{
		maxLinearV = v;
	}

	// Set body angluar speed
	void SetAngularVelocity(reVec2 v)
	{
		//angularV += v;
	}

	// Set body radius
	void SetRadius(float rad)
	{
		radius = rad;
	}

	void SetBodyAngle(double a)
	{
		angle = a;
	}

	// ===================================================
	//					Getters
	// ===================================================

	// Get the body position
	inline reVec2& GetPosition()
	{
		return position;
	}

	// Get the body linear velocity
	inline reVec2& GetLinearVelocity()
	{
		return linearV;
	}

	inline reVec2& GetMaxLinearVelocity()
	{
		return maxLinearV;
	}

	// Get the body angular velocity
	inline reVec2& GetAngularVelocity()
	{
		//return angularV;
	}

	// Get the body acceleration
	inline reVec2& GetAcceleration()
	{
		return acceleration;
	}

	// Get body mass
	inline float& GetBodyMass()
	{
		return mass;
	}

	//Get gravity on the body;
	inline float& GetBodyGravity()
	{
		return bodyGravity;
	}

	// Get the body angle
	inline float& GetBodyAngle()
	{
		return angle;
	}

	// Get the body radius
	inline float& GetBodyRadius()
	{
		return radius;
	}

	inline SString& GetName()
	{
		return name;
	}
};