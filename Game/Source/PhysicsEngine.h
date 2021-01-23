#pragma once

#include "SString.h"
#include "Module.h"
#include "p2List.h"
#include "reVec2.h"
#include "reBody.h"

class reVec2;

class PhysicsEngine : public Module
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	reVec2 forceGrav(float gravity, float mass1, float mass2, float distance, reVec2 direction);

	reVec2 Gravity();
	reVec2 forceAeroDrag(reBody* b);
	reVec2 forceAeroLift(reBody* b);
	reVec2 forceHydroBuoy(reBody* b);
	reVec2 forceHydroDrag(reBody* b);

	void step(float dt);

	bool Intersection(reBody* b1, reBody* b2);
	void Collisions(reBody* b, reBody* b2);


	reBody* CreateBody(SString n, BodyType type);
	void DeleteBody(reBody* b);

private:

	void Integrator(reVec2& x, reVec2& v, reVec2& a, float dt);

private:

	reVec2 earthGravity;
	reVec2 moonGravity;
	float aeroDrag;
	float aeroLift;
	float hydroBuoy;
	float hydroDrag;

	p2List<reBody*> bodyList;

};