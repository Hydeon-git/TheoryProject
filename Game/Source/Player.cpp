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

	body = app->physics->CreateBody(reBodyType::EARTH_GRAVITY);

	body->SetPosition(10, 10);
	body->SetLinearVelocity(0, 0);
	body->SetAngle(0);
	body->SetMass(0.1);

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

	if ((body->GetAngle() * RADTODEG >= 360) || (body->GetAngle() * RADTODEG <= -360))
	{
		body->SetAngle(0);
	}
	// Player Movement
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !moonAnim && !deadAnim)
	{
		float a = body->GetAngle();
		body->AddMomentum(a, dt);
		//a = a * RADTODEG;
		launching = true;
	}
	else launching = false;

	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (rotation) && !moonAnim && !deadAnim)
	{
		float b = body->GetAngle();

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
				float ang = body->GetAngle();
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
	else if (app->render->camera.y >= 0)
	{
		if (pos.y >= 10)
		{
			if (body->GetLinearVelocity().y < 20)
			{
				float ang = body->GetAngle();
				ang = ang * RADTODEG;
				if (((ang > 340 && ang <= 0) || (ang < 20 && ang >= 0) || (ang < -340 && ang >= 0) || (ang > -20 && ang <= 0)))
				{
					body->SetPosition(pos.x, 10);
					if (moon)
					{
						body->SetLinearVelocity(0, 0);
						finished = true;
					}
					
				}
				else
				{
					body->SetAngle(0);
					body->SetPosition(pos.x, 10);
					body->SetLinearVelocity(0, body->GetLinearVelocity().y);
					if (!lost) deadAnim = true;
				}
			}
			else
			{
				body->SetAngle(0);
				body->SetPosition(pos.x, 10);
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
		body->type = reBodyType::NO_GRAVITY;
		LOG("-------------------------");
		LOG("Leaving Earth planet ");
		LOG("Entring the outer space ");
	}
	if ((pos.y >= -50) && (outerSpace))
	{
		outerSpace = false;
		earthLeft = false;		
		body->type = reBodyType::EARTH_GRAVITY;
		LOG("-------------------------");
		LOG("Leaving the outer space ");
		LOG("Entring Earth planet ");
	}
	if ((pos.y <= -135) && (outerSpace))
	{
		outerSpace = false;
		moonLeft = false;
		body->type = reBodyType::MOON_GRAVITY;
		LOG("------------------------- ");
		LOG("Entring moon athmosphere ");
	}
	if ((pos.y >= -135) && (!moonLeft))
	{
		moonLeft = true;
		outerSpace = true;
		body->type = reBodyType::NO_GRAVITY;
		LOG("------------------------- ");
		LOG("Leaving moon athmosphere ");
		LOG("Entring the outer space ");
	}	
	// --------------------------------------------------
	
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
			body->SetPosition(10, 10);
			body->SetLinearVelocity(0,0);
			body->SetAngle(0);
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
			body->SetPosition(10, 10);
			body->SetLinearVelocity(0,0);
			body->SetAngle(0);
			body->type = reBodyType::EARTH_GRAVITY;
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
	if (!launching && !deadAnim) app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetAngle() * RADTODEG);
	else if (launching) app->render->DrawTexture(fireSpaceship, METERS_TO_PIXELS(body->GetPosition().x-18), METERS_TO_PIXELS(body->GetPosition().y-35), &rect, 1.0f, body->GetAngle() * RADTODEG);
	
	
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

	if (lost)
	{
		app->render->DrawTexture(loseScene, 0, -8000 + SCREEN_HEIGHT);
		app->render->DrawTexture(loseScene, 0, 0);
	}



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
