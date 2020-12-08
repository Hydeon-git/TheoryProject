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

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* img;
};

#endif // __PHYSICS_H__