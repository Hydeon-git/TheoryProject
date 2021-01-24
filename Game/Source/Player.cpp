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
	astronautAnim.speed = 0.001f;

	explosionAnim.PushBack({   0,  0,  88,88 });
	explosionAnim.PushBack({  88, 0, 88,88 });
	explosionAnim.PushBack({ 176,0, 88,88 });
	explosionAnim.PushBack({ 264,0, 88,88 });
	explosionAnim.PushBack({ 0, 88, 88,88 });
	explosionAnim.PushBack({ 88,88,88,88 });
	explosionAnim.PushBack({ 176,88,88,88 });
	explosionAnim.PushBack({ 264,88,88,88 });
	explosionAnim.PushBack({ 0, 176, 88,88 });
	explosionAnim.PushBack({ 88,176,88,88 });
	explosionAnim.PushBack({ 176,176,88,88 });
	explosionAnim.PushBack({ 264,176,88,88 });
	explosionAnim.PushBack({ 0, 264, 88,88 });
	explosionAnim.PushBack({ 88,264,88,88 });
	explosionAnim.PushBack({ 176,264,88,88 });
	explosionAnim.PushBack({ 264,264,88,88 });
	explosionAnim.loop = false;
	explosionAnim.speed = 0.01f;

	flagAnim.PushBack({ 0,0,46,54 });
	flagAnim.PushBack({ 47,0,46,54 });
	flagAnim.speed = 0.001;
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
	explosion = app->tex->Load("Assets/Textures/explosion.png");
	flag = app->tex->Load("Assets/Textures/flag.png");
	loseScene = app->tex->Load("Assets/Textures/loseScene.png");
	winScene = app->tex->Load("Assets/Textures/winScene.png");

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

	if ((body->GetBodyAngle() * RADTODEG >= 360) || (body->GetBodyAngle() * RADTODEG <= -360))
	{
		body->SetBodyAngle(0);
	}
	// Player Movement
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !moonAnim && !deadAnim)
	{
		float a = body->GetBodyAngle();
		body->AddMomentum(a, dt);
		//a = a * RADTODEG;
		launching = true;
	}
	else launching = false;

	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (rotation) && !moonAnim && !deadAnim)
	{
		float b = body->GetBodyAngle();

		body->AddNegativeMomentum(b, dt);
	}
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (rotation) && !moonAnim && !deadAnim)
	{
		body->Rotate(90 * dt);
	}
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (rotation) && !moonAnim && !deadAnim)
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
		if (pos.y <= -143)
		{
			if (body->GetLinearVelocity().y > -20)
			{
				float ang = body->GetBodyAngle();
				ang = ang * RADTODEG;
				if (((ang > 160 && ang < 200) || (ang < -160 && ang > -200)))
				{
					moon = true;
					body->SetPosition(pos.x, -143);
					body->SetLinearVelocity(0, body->GetLinearVelocity().y);
					if (!flagMoon) moonAnim = true;
				}
				else
				{
					body->SetPosition(pos.x, -143);
					body->SetLinearVelocity(0, body->GetLinearVelocity().y);
					if (!lost) deadAnim = true;
				}
			}
			else
			{
				body->SetPosition(pos.x, -143);
				body->SetLinearVelocity(0, body->GetLinearVelocity().y);
				if (!lost) deadAnim = true;
			}
		}
	}
	if (!launched && !app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		body->SetLinearVelocity(0, 0);
	}
	// Can rotate?
	if ((pos.y <= 10) && (pos.y >= -143)) launched = true;
	else launched = false;
	
	if ((pos.y <= 9) && (pos.y >= -142)) rotation = true;
	else rotation = false;
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
	if ((pos.y >= -50) && (outerSpace))
	{
		outerSpace = false;
		earthLeft = false;		
		body->type = BodyType::EARTH_GRAVITY;
		LOG("-------------------------");
		LOG("Leaving the outer space ");
		LOG("Entring Earth planet ");
	}
	if ((pos.y <= -135) && (outerSpace))
	{
		outerSpace = false;
		moonLeft = false;
		body->type = BodyType::MOON_GRAVITY;
		LOG("------------------------- ");
		LOG("Entring moon athmosphere ");
	}
	if ((pos.y >= -135) && (!moonLeft))
	{
		moonLeft = true;
		outerSpace = true;
		body->type = BodyType::NO_GRAVITY;
		LOG("------------------------- ");
		LOG("Leaving moon athmosphere ");
		LOG("Entring the outer space ");
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
			launched = false;
			earthLeft = false;
			outerSpace = false;
			moonLeft = true;
			reVec2 pos2 = reVec2(10, 10);
			body->SetPosition(pos2);
			body->SetLinearVelocity(0,0);
			body->SetBodyAngle(0);
		}
	}
	// Lose Scancode
	if (lost)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			moon = false;
			lost = false;
			launched = false;
			earthLeft = false;
			outerSpace = false;
			moonLeft = true;
			reVec2 pos2 = reVec2(10, 10);
			body->SetPosition(pos2);
			body->SetLinearVelocity(0,0);
			body->SetBodyAngle(0);
			body->type = BodyType::EARTH_GRAVITY;
		}
	}

	if (moonAnim) astronautAnim.Update();
	if(deadAnim) explosionAnim.Update();
	flagAnim.Update();
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = { 0,0,58,141 };
	if (!launching && !deadAnim) app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetBodyAngle() * RADTODEG);
	else if (launching) app->render->DrawTexture(fireSpaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetBodyAngle() * RADTODEG);
	
	
	if (moonAnim && !flagMoon) app->render->DrawTexture(astronaut, METERS_TO_PIXELS(body->GetPosition().x + 45), METERS_TO_PIXELS(body->GetPosition().y - 25), &(astronautAnim.GetCurrentFrame()), 1.0f);
	else if (flagMoon) app->render->DrawTexture(flag, METERS_TO_PIXELS(body->GetPosition().x + 120), METERS_TO_PIXELS(body->GetPosition().y - 10), &(flagAnim.GetCurrentFrame()), 1.0f);
	if (deadAnim) app->render->DrawTexture(explosion, METERS_TO_PIXELS(body->GetPosition().x - 22), METERS_TO_PIXELS(body->GetPosition().y), &(explosionAnim.GetCurrentFrame()), 1.0f);

	if (astronautAnim.HasFinished())
	{
		moonAnim = false;
		flagMoon = true;
		launched = false;
		astronautAnim.Reset();
	}
	if (explosionAnim.HasFinished())
	{
		deadAnim = false;
		lost = true;
		explosionAnim.Reset();
	}

	if (lost) app->render->DrawTexture(loseScene, 0, -8000 + SCREEN_HEIGHT);
	if (finished) app->render->DrawTexture(winScene, 0, 0);

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
