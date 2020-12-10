#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"

struct SDL_Texture;

class Physics : public Module
{
public:

	Physics();

	// Destructor
	virtual ~Physics();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Physics Functions
	void VelocityVerlet(double &x, double &v, double a, double dt);

private:
	SDL_Texture* spaceshipTex;
};

#endif // __PHYSICS_H__