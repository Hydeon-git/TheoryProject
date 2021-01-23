#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "Input.h"

Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake()
{
	//LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	spaceship = app->tex->Load("Assets/Textures/motorcito.png");
	finish = app->tex->Load("Assets/Textures/finish.png");

	body = app->physics->CreateBody("player", BodyType::DYNAMIC);

	body->SetPosition(reVec2(PIXEL_TO_METERS(500), PIXEL_TO_METERS(0)));
	body->SetLinearVelocity(reVec2(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)));
	body->SetMass(0.1);
	body->SetRadius(PIXEL_TO_METERS(18));
	body->SetMaxLinearVelocity(reVec2(PIXEL_TO_METERS(500), PIXEL_TO_METERS(500)));
	body->SetBodyAngle(0);
	//fuel = 50.0f;

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT )
	{
		float a = body->GetBodyAngle();
		body->AddMomentum(a);
	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		/*float ang = body->GetBodyAngle();
		reVec2 mom = reVec2(cos(ang - 90 * M_PI / 180), sin(ang - 90 * M_PI / 180));
		body->AddMomentum(reVec2(PIXEL_TO_METERS(-mom.x * 0.00016 * 250), PIXEL_TO_METERS(-mom.y * 0.00016 * 250)));*/
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		body->Rotate(2* 0.0016);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		body->Rotate(-2* 0.0016);
	}


	reVec2 pos = body->GetPosition();
	
	if (app->render->camera.y <= 0 && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT)
	{
		reVec2 pos2 = reVec2(pos.x, 10);
		body->SetPosition(pos2);
		initialPos = true;
		body->SetLinearVelocity(0, 0);
	}
	
	if (app->render->camera.y >= 8000 - SCREEN_HEIGHT)
	{
		reVec2 pos2 = reVec2(pos.x, -8000);
		body->SetPosition(pos2);
		finished = true;
	}
	if (finished)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			finished = false;
			reVec2 pos2 = reVec2(10, 10);
			body->SetPosition(pos2);
			body->SetLinearVelocity(0,0);
		}

	}
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	//app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x), METERS_TO_PIXELS(body->GetPosition().y));
	SDL_Rect rect = { 0,0,58,98 };
	app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x - 18), METERS_TO_PIXELS(body->GetPosition().y - 15), &rect, 1.0f, body->GetBodyAngle() * 180 / PI);
	if (finished) app->render->DrawTexture(finish, 0, -8000 + SCREEN_HEIGHT);
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

 //Called before quitting
bool Player::CleanUp()
{
	//LOG("Freeing scene");

	return true;
}
