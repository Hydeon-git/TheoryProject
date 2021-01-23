#include "App.h"
#include "Player.h"

#include "PhysicsEngine.h"
#include <math.h>

PhysicsEngine::PhysicsEngine() : Module()
{
	gravity = reVec2(PIXEL_TO_METERS(0), PIXEL_TO_METERS(100));
	aeroDrag = 0.3f;
	aeroLift = 0.3f;
	hydroDrag = 0.3f;
}

PhysicsEngine::~PhysicsEngine()
{
}

// Called before render is available
bool PhysicsEngine::Awake()
{
	//LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PhysicsEngine::Start()
{
	
	return true;
}

// Called each loop iteration
bool PhysicsEngine::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PhysicsEngine::Update(float dt)
{
	step(0.00016);

	return true;
}

// Called each loop iteration
bool PhysicsEngine::PostUpdate()
{
	return true;
}
// Called before quitting
bool PhysicsEngine::CleanUp()
{
	bodyList.clear();
	return true;
}
reVec2 PhysicsEngine::forceGrav(float gravity, float mass1, float mass2, float distance, reVec2 direction)
{
	reVec2 forceGravity;

	float gravityFormule = (gravity * (mass1 * mass2) / (distance * distance));

	forceGravity.x = direction.x * gravityFormule;
	forceGravity.y = direction.y * gravityFormule;

	return forceGravity;
}

reVec2 PhysicsEngine::Gravity()
{
	return gravity;
}

reVec2 PhysicsEngine::forceAeroDrag(reBody* b)
{
	float density = b->GetBodyMass() /*b->*/;

	reVec2 dragForce;

	float x = aeroDrag /* Area*/ * ((density * (b->GetLinearVelocity().x * b->GetLinearVelocity().x)) / 2);
	float y = aeroDrag /* Area*/ * ((density * (b->GetLinearVelocity().y * b->GetLinearVelocity().y)) / 2);

	dragForce = reVec2(x, y);

	return dragForce;
}

reVec2 PhysicsEngine::forceAeroLift(reBody* b)
{
	float density = b->GetBodyMass() /*volumen*/;

	reVec2 liftForce;

	float x = (density * (b->GetLinearVelocity().x * b->GetLinearVelocity().x) / 2) * aeroLift; /*area*/
	float y = (density * (b->GetLinearVelocity().y * b->GetLinearVelocity().y) / 2) * aeroLift; /*area*/

	liftForce = reVec2(x, y);

	return liftForce;
}

reVec2 PhysicsEngine::forceHydroBuoy(reBody* b)
{
	reVec2 hydroBuoyForce;

	return hydroBuoyForce;
}

reVec2 PhysicsEngine::forceHydroDrag(reBody* b)
{
	float density = b->GetBodyMass() /*b->*/;

	reVec2 hydroDragForce;

	float x = hydroDrag /* Area*/ * ((density * (b->GetLinearVelocity().x * b->GetLinearVelocity().x)) / 2);
	float y = hydroDrag /* Area*/ * ((density * (b->GetLinearVelocity().y * b->GetLinearVelocity().y)) / 2);

	hydroDragForce = reVec2(x, y);

	return hydroDragForce;
}

void PhysicsEngine::step(float dt)
{
	p2List_item<reBody*>* item = bodyList.getFirst();
	while (item != nullptr)
	{
		if(item->data->type == BodyType::DYNAMIC && item->data->IsActive())
			Integrator(item->data->GetPosition(), item->data->GetLinearVelocity(), item->data->GetAcceleration() + gravity, dt);

		else if(item->data->type == BodyType::NO_GRAVITY && item->data->IsActive())
			Integrator(item->data->GetPosition(), item->data->GetLinearVelocity(), item->data->GetAcceleration(), dt);

		item = item->next;
	}

	/*else if (app->spaceship->GetBody()->GetPosition().y <= 0)
		gravity.y = 9.81f - ((app->scene->floor->GetPosition().y - app->spaceship->GetBody()->GetPosition().y) / 9.81f);*/

	//LOG("gravity = %f", gravity.y);

	for (p2List_item<reBody*>* item = bodyList.getFirst(); item != nullptr; item = item->next)
	{
		for (p2List_item<reBody*>* item2 = item->next; item2 != nullptr; item2 = item2->next)
		{
			if (Intersection(item->data, item2->data) && item->data->IsActive())
			{
				Collisions(item->data, item2->data);
			}
		}
	}
}

bool PhysicsEngine::Intersection(reBody* b1, reBody* b2)
{
	float x = b2->GetPosition().x - (b1->GetPosition().x);
	float y = b2->GetPosition().y - (b1->GetPosition().y);
	float dist = sqrt(pow(x, 2) + pow(y, 2));

	if (dist < b1->GetBodyRadius() + b2->GetBodyRadius())
		return true;
	else
		return false;
}

void PhysicsEngine::Collisions(reBody* b, reBody* b2)
{
	
	// First we get the normal corresponding from the world to the body
	reVec2 dir = b->GetPosition() - b2->GetPosition();
	reVec2 dirNoNormalized = dir;

	// We normalize the vector, since we only want the direction
	dir = dir.Normalize();
	
	// We get the actual velocity of the spaceship
	reVec2 newSpeed = b->GetLinearVelocity();

	// We get the speed as a float
	float speed = sqrt((newSpeed.x * newSpeed.x) + (newSpeed.y * newSpeed.y));

	// We now multiply the speed for the direction so the spaceship knows the direction where it has to go to
	newSpeed = dir * speed;

	if (b2->GetName() == "floor")
		newSpeed = newSpeed * 0.6f;
	
	else
		newSpeed = newSpeed * 0.9f;

	b->SetLinearVelocity(newSpeed);




	/*if (b2->GetName() == "astronaut")
	{
		app->astronautManager->DeleteAstronaut(b2);
		app->spaceship->AddScore();
	}
	else
	{
		b->SetLinearVelocity((newSpeed));
	}*/


}
reBody* PhysicsEngine::CreateBody(SString n, BodyType type)
{
	reBody* b = new reBody(n, type);

	bodyList.add(b);

	return b;
}

void PhysicsEngine::DeleteBody(reBody* b)
{
	p2List_item<reBody*>* item = bodyList.getFirst();

	while (item != nullptr)
	{
		if (item->data == b)
		{
			bodyList.del(item);
		}

		item = item->next;
	}
}

void PhysicsEngine::Integrator(reVec2& pos, reVec2& v, reVec2& a, float dt)
{

	// Velocity-Verlet
	pos.x += v.x * dt + 0.5 * a.x * dt * dt;
	pos.y += v.y * dt + 0.5 * a.y * dt * dt;

	v.x += a.x * dt;
	v.y += a.y * dt;

}