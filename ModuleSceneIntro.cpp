#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	Cube* cube;
	platOffset = 100;

	//CHECKPOINTS
	{
		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(0, 10 + platOffset, 0);
		cube->SetRotation(0, { 0,1,0 });
		checkpoints.add(cube);
	}
	/*ground = new Plane(0, 1, 0, 0);
	ground->axis = false;*/
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();




	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)App->SetDebug();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	switch (body2->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision CHECKPOINT");
		for (size_t i = 0; i < checkPlat.count(); i++)
		{
			if (body1 == checkPlat.at(i).data || body2 == checkPlat.at(i).data)
				App->player->lastCheckpoint = i;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



	switch (body1->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision CHECKPOINT");
		for (size_t i = 0; i < checkPlat.count(); i++)
		{
			if (body1 == checkPlat.at(i).data || body2 == checkPlat.at(i).data)
				App->player->lastCheckpoint = i;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

