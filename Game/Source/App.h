#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "p2List.h"
#include "Timer.h"


// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class PhysicsEngine;
class Player;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

private:


	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	PhysicsEngine* physics;
	Player* player;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	p2List<Module *> modules;

	Timer frameTime;
	uint frames;
public:
	float dt = 0.0f;
};

extern App* app;

#endif	// __APP_H__