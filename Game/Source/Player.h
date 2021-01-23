#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "reBody.h"
#include "reVec2.h"
#include "Module.h"

struct SDL_Texture;

class Player : public Module
{
public:

	Player();
	// Destructor
	virtual ~Player();

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

	inline reBody* GetBody()
	{
		return body;
	}

private:
	SDL_Texture* spaceship;
	SDL_Texture* fireSpaceship;
	SDL_Texture* finish;
	reBody* body;
	bool finished = false;
	bool moon = false;
	bool launching = false;
};

#endif // __PLAYER_H__