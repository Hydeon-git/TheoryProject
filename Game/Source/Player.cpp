#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "Input.h"

Player::Player() : Module()
{
	name.Create("player");

	astronautAnim.PushBack({ 0,0,68,85 });
	astronautAnim.PushBack({ 69,0,64,85 });
	astronautAnim.PushBack({ 133,0,67,88 });
	astronautAnim.loop = false;
	astronautAnim.speed = 0.0001f;

	flagAnim.PushBack({ 0,0,46,54 });
	flagAnim.PushBack({ 47,0,46,54 });
	flagAnim.speed = 0.0001f;
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
	spaceship = app->tex->Load("Assets/Textures/spaceship.png");
	fireSpaceship = app->tex->Load("Assets/Textures/fireSpaceship.png");
	astronaut = app->tex->Load("Assets/Textures/astronaut.png");
	flag = app->tex->Load("Assets/Textures/flag.png");
	finish = app->tex->Load("Assets/Textures/finish.png");

	body = app->physics->CreateBody("player", BodyType::EARTH_GRAVITY);

	body->SetPosition(reVec2(PIXEL_TO_METERS(500), PIXEL_TO_METERS(500)));
	body->SetLinearVelocity(reVec2(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)));
	body->SetMass(0.1);
	body->SetRadius(PIXEL_TO_METERS(18));
	body->SetMaxLinearVelocity(reVec2(PIXEL_TO_METERS(500), PIXEL_TO_METERS(500)));
	body->SetBodyAngle(0);

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
	// --------------------------------------------------
	// Get Player Position
	reVec2 pos = body->GetPosition();
	// Player Movement
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		float a = body->GetBodyAngle();
		body->AddMomentum(a, dt);
		launching = true;
	}
	else launching = false;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		float b = body->GetBodyAngle();
		body->AddNegativeMomentum(b, dt);
		launching = true;
	}

	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (launched))
	{
		body->Rotate(90 * dt);
	}
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (launched))
	{
		body->Rotate(-90 * dt);
	}
	// --------------------------------------------------
	// Limits
	if (pos.x < 0)
	{
		body->SetPosition(0, pos.y);
		body->SetLinearVelocity(0, body->GetLinearVelocity().y);
	}
	if (pos.x > PIXEL_TO_METERS(SCREEN_WIDTH - 0.5))
	{
		body->SetPosition(PIXEL_TO_METERS(SCREEN_WIDTH - 0.5), pos.y);
		body->SetLinearVelocity(0, body->GetLinearVelocity().y);
	}
	if (app->render->camera.y >= 8000 - SCREEN_HEIGHT)
	{
		if (pos.y <= -143 )
		{
			body->SetPosition(pos.x, -143);
			if (!moon)
			{
				body->SetLinearVelocity(0, 0);
			}
			body->SetLinearVelocity(0, body->GetLinearVelocity().x);
		}
		moon = true;
	}
	if (pos.y <= 9)
	{
		launched = true;
	}
	// --------------------------------------------------
	// Gravity Changes
	if ((pos.y <= -50) && (!earthLeft))
	{
		earthLeft = true;
		outerSpace = true;
		body->type = BodyType::NO_GRAVITY;
		LOG("-------------------------");
		LOG("Leaving Earth planet ");
		LOG("Entring the outer space ");
	}
	if ((pos.y <= -135) && (outerSpace))
	{
		outerSpace = false;
		moonLeft = false;
		body->type = BodyType::MOON_GRAVITY;
		LOG("------------------------- ");
		LOG("Entring moon athmosphere ");
	}
	// --------------------------------------------------
	// Win Condition
	if (pos.y >= 10)
	{
		body->SetPosition(pos.x, 10);
		if (moon)
		{
			body->SetLinearVelocity(0, 0);
			finished = true;
		}
	}
	// Win Scancode
	if (finished)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			moon = false;
			finished = false;
			reVec2 pos2 = reVec2(10, 10);
			body->SetPosition(pos2);
			body->SetLinearVelocity(0,0);
		}
	}

	astronautAnim.Update();
	flagAnim.Update();
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = { 0,0,58,141 };
	if (!launching) app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetBodyAngle() * 180 / PI);
	else if (launching) app->render->DrawTexture(fireSpaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetBodyAngle() * 180 / PI);
	if (finished) app->render->DrawTexture(finish, 0, 0);
	if (!landedMoon) app->render->DrawTexture(astronaut, METERS_TO_PIXELS(body->GetPosition().x + 45), METERS_TO_PIXELS(body->GetPosition().y - 25), &(astronautAnim.GetCurrentFrame()), 1.0f);
	else if (landedMoon) app->render->DrawTexture(flag, METERS_TO_PIXELS(body->GetPosition().x + 120), METERS_TO_PIXELS(body->GetPosition().y - 10), &(flagAnim.GetCurrentFrame()), 1.0f);

	if (astronautAnim.HasFinished())
	{
		landedMoon = true;
		astronautAnim.Reset();
	}
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
