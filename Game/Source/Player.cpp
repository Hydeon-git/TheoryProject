#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "Input.h"
#include "Audio.h"


Player::Player() : Module()
{
	name.Create("player");

	astronautAnimR.PushBack({ 0,0,68,85 });
	astronautAnimR.PushBack({ 69,0,64,85 });
	astronautAnimR.PushBack({ 133,0,67,88 });
	astronautAnimR.loop = false;
	astronautAnimR.speed = 2.0f;
	
	astronautAnimL.PushBack({ 201,0,68,85 });
	astronautAnimL.PushBack({ 269,0,63,85 });
	astronautAnimL.PushBack({ 334,0,66,88 });
	astronautAnimL.loop = false;
	astronautAnimL.speed = 2.0f;

	explosionAnim.PushBack({ 0,  0,  88,88 });
	explosionAnim.PushBack({ 88, 0, 88,88 });
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
	explosionAnim.speed = 14.0f;

	flagAnim.PushBack({ 0,0,46,54 });
	flagAnim.PushBack({ 47,0,46,54 });
	flagAnim.speed = 3.0f;
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
	// --------------------------------------------------
	// Texture loading
	spaceship = app->tex->Load("Assets/Textures/spaceship.png");
	fireSpaceship = app->tex->Load("Assets/Textures/fireSpaceship.png");
	astronaut = app->tex->Load("Assets/Textures/astronaut.png");
	explosion = app->tex->Load("Assets/Textures/explosion.png");
	flag = app->tex->Load("Assets/Textures/flag.png");
	loseScene = app->tex->Load("Assets/Textures/loseScene.png");
	winScene = app->tex->Load("Assets/Textures/winScene.png");
	// --------------------------------------------------
	// SoundFx loading
	explosionFx = app->audio->LoadFx("Assets/Audio/Fx/explosion.ogg");
	loseFx = app->audio->LoadFx("Assets/Audio/Fx/lose.ogg");
	winFx = app->audio->LoadFx("Assets/Audio/Fx/win.ogg");
	landedFx = app->audio->LoadFx("Assets/Audio/Fx/eagleLanded.ogg");
	// --------------------------------------------------
	// Create Spaceship and set properties
	body = app->physics->createBody(reBodyType::EARTH_GRAVITY);

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
	// Get Player Position and set angle
	reVec2 pos = body->GetPosition();
	
	if ((body->GetAngle() * RADTODEG >= 360) || (body->GetAngle() * RADTODEG <= -360))
	{
		body->SetAngle(0);
	}
	// --------------------------------------------------
	// Player Movement
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && (!moonAnim) && (!deadAnim) && (!lost) && (!finished))
	{
		float a = body->GetAngle();
		body->AddMomentum(a, dt);
		launching = true;
	}
	else launching = false;	
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && (rotation) && (!moonAnim) && (!deadAnim))
	{
		float b = body->GetAngle();

		body->AddNegativeMomentum(b, dt);
	}
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (rotation) && (!moonAnim) && (!deadAnim))
	{
		body->Rotate(90 * dt);
	}
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (rotation) && (!moonAnim) && (!deadAnim))
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
	// --------------------------------------------------
	// Check Ship Angles and Velocity
	if (app->render->camera.y >= 8000 - SCREEN_HEIGHT)
	{	
		if (pos.y <= -143)
		{
			if (!flagMoon)
			{
				// Get player player position for astronaut animation
				if (pos.x < PIXEL_TO_METERS(SCREEN_WIDTH / 2))
				{
					moonPlayerLeft = true;
					flagPosX = METERS_TO_PIXELS(body->GetPosition().x + 120);
					astronautPosX = METERS_TO_PIXELS(body->GetPosition().x + 45);
				}
				else
				{
					moonPlayerLeft = false;
					flagPosX = METERS_TO_PIXELS(body->GetPosition().x - 130);
					astronautPosX = METERS_TO_PIXELS(body->GetPosition().x - 90);
				}
				flagPosY = METERS_TO_PIXELS(body->GetPosition().y - 10);
				astronautPosY = METERS_TO_PIXELS(body->GetPosition().y - 25);
			}

			if (body->GetLinearVelocity().y > -20)
			//if (body->GetLinearVelocity().y > -20 && !deadAnim && !lost)
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
					body->SetLinearVelocity(0, 0);
					explosionEffect = true;
					if (!lost) deadAnim = true;
				}
			}
			else
			{				
				body->SetLinearVelocity(0, 0); 
				explosionEffect = true;
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
						winTimer.Start();
						while (winCount <= 3.0f)
						{
							body->SetLinearVelocity(0, 0);
							winCount = winTimer.Read() * 0.001f;							
						}						
						finished = true;
					}					
				}
				else
				{
					moon = false;
					body->SetAngle(0);
					body->SetLinearVelocity(0, 0);
					explosionEffect = true;
					if (!lost) deadAnim = true;
				}
			}
			else
			{
				moon = false;
				body->SetAngle(0);
				body->SetLinearVelocity(0, 0); 
				explosionEffect = true;
				if (!lost) deadAnim = true;
			}
		}
	}
	// --------------------------------------------------
	// Linear velocity to 0 when we are stationed
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
	// Gravity Zones
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
			// Bool resets
			moon = false;
			lost = false;
			finished = false;
			launched = false;
			earthLeft = false;
			outerSpace = false;
			moonLeft = true;
			flagMoon = false;

			// Timer reset
			winCount = 0.0f;
		
			// Player reset
			body->SetPosition(10, 10);
			body->SetLinearVelocity(0,0);
			body->SetAngle(0);
			app->audio->StopFx(winFx);
			app->audio->PlayMusic("Assets/Audio/Music/landing.ogg", 80);
		}
	}
	// Lose Scancode
	if (lost)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			moon = false;
			lost = false;
			flagMoon = false;
			launched = false;
			earthLeft = false;
			outerSpace = false;
			moonLeft = true;
			body->SetPosition(10, 10);
			body->SetLinearVelocity(0,0);
			body->SetAngle(0);
			body->type = reBodyType::EARTH_GRAVITY;
			app->audio->PlayMusic("Assets/Audio/Music/landing.ogg", 80);
		}
	}

	// Animations HasFinished and Update
	if (astronautAnimR.HasFinished())
	{
		moonAnim = false;
		flagMoon = true;
		launched = false;
		astronautAnimR.Reset();
	}
	if (explosionAnim.HasFinished())
	{
		deadAnim = false;
		lost = true;
		loseEffect = true;
		body->SetPosition(10, 10);
		explosionAnim.Reset();
	}
	if (moonAnim) astronautAnimR.Update(dt);
	if (moonAnim) astronautAnimL.Update(dt);
	if (deadAnim) explosionAnim.Update(dt);
	flagAnim.Update(dt);
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = { 0,0,58,141 };
	if (!launching && !deadAnim && !lost) app->render->DrawTexture(spaceship, METERS_TO_PIXELS(body->GetPosition().x - 18), METERS_TO_PIXELS(body->GetPosition().y - 35), &rect, 1.0f, body->GetAngle() * RADTODEG);
	else if (launching && !lost) app->render->DrawTexture(fireSpaceship, METERS_TO_PIXELS(body->GetPosition().x - 18), METERS_TO_PIXELS(body->GetPosition().y - 35), &rect, 1.0f, body->GetAngle() * RADTODEG);

	if (moonPlayerLeft)
	{
		if (moonAnim && !flagMoon) app->render->DrawTexture(astronaut, astronautPosX, astronautPosY, &(astronautAnimR.GetCurrentFrame()), 1.0f);
		else if (flagMoon) app->render->DrawTexture(flag, flagPosX, flagPosY, &(flagAnim.GetCurrentFrame()), 1.0f);
	}
	else
	{
		if (moonAnim && !flagMoon) app->render->DrawTexture(astronaut, astronautPosX, astronautPosY, &(astronautAnimL.GetCurrentFrame()), 1.0f);
		else if (flagMoon) app->render->DrawTexture(flag, flagPosX, flagPosY, &(flagAnim.GetCurrentFrame()), 1.0f);
	}

	if (deadAnim)
	{
		if (explosionEffect)
		{
			app->audio->PlayFx(explosionFx, 30);
			explosionEffect = false;
		}

		app->render->DrawTexture(explosion, METERS_TO_PIXELS(body->GetPosition().x - 22), METERS_TO_PIXELS(body->GetPosition().y), &(explosionAnim.GetCurrentFrame()), 1.0f);
	}
	if (moonAnim && !flagMoon) app->audio->PlayFx(landedFx, 30);
	if (lost)
	{
		if (loseEffect)
		{
			app->audio->PlayFx(loseFx, 50);
			loseEffect = false;
		}
		app->render->DrawTexture(loseScene, 0, 0);
	}

	if (finished)
	{
		app->audio->StopMusic();
		app->audio->PlayFx(winFx, 10);
		app->render->DrawTexture(winScene, 0, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

 //Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
