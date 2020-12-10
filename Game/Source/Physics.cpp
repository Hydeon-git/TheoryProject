#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Physics::Physics() : Module()
{
	name.Create("physics");
}

// Destructor
Physics::~Physics()
{}

// Called before render is available
bool Physics::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Physics::Start()
{

	return true;
}

// Called before quitting
bool Physics::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Physics::VelocityVerlet(double& x, double& v, double a, double dt)
{
	x += v * dt + 0.5 * a * dt * dt; // Update position
	v += a * dt; // Update velocity
}
