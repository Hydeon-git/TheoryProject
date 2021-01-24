#include "App.h"
#include "Player.h"

#include "PhysicsEngine.h"
#include <math.h>

PhysicsEngine::PhysicsEngine() : Module()
{
	
}

PhysicsEngine::~PhysicsEngine()
{
}

// Called before the first frame
bool PhysicsEngine::Start()
{
	earthGravity = reVec2(PIXEL_TO_METERS(0), PIXEL_TO_METERS(98));
	moonGravity = reVec2(PIXEL_TO_METERS(0), PIXEL_TO_METERS(16));
	
	return true;
}

// Called each loop iteration
bool PhysicsEngine::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PhysicsEngine::Update(float dt)
{
	step(dt);

	return true;
}

// Called each loop iteration
bool PhysicsEngine::PostUpdate()
{
	return true;
}
// Called before quitting
bool PhysicsEngine::CleanUp()
{
	p2List_item<reBody*>* item = reBodyList.getFirst();
	while (item != nullptr)
	{
		reBodyList.del(item);
		item = item->next;
	}
	reBodyList.clear();

	return true;
}
reVec2 PhysicsEngine::forceGrav(float mass1, float mass2, float r, reVec2 dir)
{
	reVec2 forceGravity;
	float G = 6.67e-11; // Gravitational constant

	float gravityFormule = (G * (mass1 * mass2) / (r * r)); // Apply formula for general gravitational force
	forceGravity.x = dir.x * gravityFormule;
	forceGravity.y = dir.y * gravityFormule;

	return forceGravity;
}

reVec2 PhysicsEngine::forceAeroDrag(reBody* b)
{
	// Using aerodynamics equation for drag force
	reVec2 aeroDrag;

	float density = 1.5f; // Air density
	reVec2 velocity = b->GetLinearVelocity(); // Body velocity
	float coAeroDrag = 0.15f; // Drag coefficient
	float s = PIXEL_TO_METERS(58) * PIXEL_TO_METERS(98); // Body area

	aeroDrag.x = (1 / 2) * density * (velocity.x * velocity.x) * s * coAeroDrag;
	aeroDrag.y = (1 / 2) * density * (velocity.y * velocity.y) * s * coAeroDrag;

	return aeroDrag;
}

reVec2 PhysicsEngine::forceAeroLift(reBody* b)
{
	// Using aerodynamics equation for lift force
	reVec2 aeroLift;

	float density = 1.5f; // Air density
	reVec2 velocity = b->GetLinearVelocity(); // Body velocity
	float coAeroLift = 0.5f; // Lift coefficient
	float s = PIXEL_TO_METERS(58) * PIXEL_TO_METERS(98); // Body area

	aeroLift.x = (1 / 2) * density * (velocity.x * velocity.x) * s * coAeroLift;
	aeroLift.y = (1 / 2) * density * (velocity.y * velocity.y) * s * coAeroLift;

	return aeroLift;
}

reVec2 PhysicsEngine::forceHydroBuoy(reBody* b)
{
	reVec2 hydroBuoy;

	return hydroBuoy;
}

reVec2 PhysicsEngine::forceHydroDrag(reBody* b)
{
	reVec2 hydroDrag;

	return hydroDrag;
}

void PhysicsEngine::step(float dt)
{
	p2List_item<reBody*>* item = reBodyList.getFirst();
	while (item != nullptr)
	{
		if (item->data->type == reBodyType::EARTH_GRAVITY && item->data->IsActive())
		{
			integrateVerlet(item->data->GetPosition(), item->data->GetLinearVelocity(), item->data->GetAcceleration() + earthGravity, dt);
		}
			
		else if (item->data->type == reBodyType::MOON_GRAVITY && item->data->IsActive())
		{
			integrateVerlet(item->data->GetPosition(), item->data->GetLinearVelocity(), item->data->GetAcceleration() - moonGravity, dt);

		}			
		else if (item->data->type == reBodyType::NO_GRAVITY && item->data->IsActive())
		{
			integrateVerlet(item->data->GetPosition(), item->data->GetLinearVelocity(), item->data->GetAcceleration(), dt);
		}

		item = item->next;
	}
}

void PhysicsEngine::integrateVerlet(reVec2& pos, reVec2& v, reVec2& a, float dt)
{
	// Velocity-Verlet
	// 2nd order

	pos.x += v.x * dt + 0.5 * a.x * dt * dt; // Update position for x (using old velocity)
	pos.y += v.y * dt + 0.5 * a.y * dt * dt; // Update position for y (using old velocity)

	v.x += a.x * dt; // Update velocity for x
	v.y += a.y * dt; // Update velocity for y
}

reBody* PhysicsEngine::createBody(reBodyType type)
{
	reBody* b = new reBody(type);

	reBodyList.add(b);

	return b;
}