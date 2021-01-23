#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "reBody.h"
#include "reVec2.h"
#include "Module.h"
#include "Log.h"
#include "Animation.h"

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
	reBody* body;

	// Textures
	SDL_Texture* spaceship;
	SDL_Texture* fireSpaceship;
	SDL_Texture* astronaut;
	SDL_Texture* flag;
	SDL_Texture* finish;

	Animation astronautAnim;
	Animation flagAnim;

	bool moon = false;
	bool finished = false;
	
	// Launch control bools
	bool launching = false;
	bool launched = false;

	// Gravity changes bools
	bool earthLeft = false;
	bool outerSpace = false;
	bool moonLeft = true;
	bool landedMoon = false;
};

#endif // __PLAYER_H__