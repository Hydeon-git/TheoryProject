#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "PhysicsEngine.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	groundTex = app->tex->Load("Assets/Textures/ground.png");
	fondo = app->tex->Load("Assets/Textures/background.png");
	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//floor = app->physics->CreateBody("floor", BodyType::STATIC);
	//floor->SetRadius(PIXEL_TO_METERS(1000));
	//int x = SCREEN_WIDTH / 2;
	//int y = 1250 + 2 * floor->GetBodyRadius();
	//floor->SetPosition(reVec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)));

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	app->render->DrawTexture(fondo, 0, -8000 + SCREEN_HEIGHT);
	//app->render->DrawTexture(groundTex, SCREEN_WIDTH / 6, 600);
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
