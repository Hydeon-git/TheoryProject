#pragma once

#include "SString.h"
#include "Module.h"
#include "p2List.h"
#include "reVec2.h"
#include "reBody.h"

//class reVec2;

class PhysicsEngine : public Module
{
public:
	PhysicsEngine();
	~PhysicsEngine();

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

	reVec2 forceGrav(float mass1, float mass2, float r, reVec2 dir); // Compute Gravity force
	reVec2 forceAeroDrag(reBody* b); // Compute Aerodynamic Drag force
	reVec2 forceAeroLift(reBody* b); // Compute Aerodynamic Lift force
	reVec2 forceHydroBuoy(reBody* b); // Compute Hydrodynamic Buoyancy force
	reVec2 forceHydroDrag(reBody* b); // Compute Hydrodynamic Drag force

	void step(float dt);  // Step physics
	void integrateVerlet(reVec2& x, reVec2& v, reVec2& a, float dt); // Integrator using Verlet

	reBody* createBody(reBodyType type);

private:
	p2List<reBody*> reBodyList;
	reVec2 earthGravity;
	reVec2 moonGravity;	
};