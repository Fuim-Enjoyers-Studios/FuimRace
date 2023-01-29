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
	PhysBody3D* temp;
	platOffset = 10;

	//CIRCUITO
	{
		cube = new Cube(100, 1, 15);
		cube->SetPos(-90, platOffset, -86);
		cube->color.Set(1, 1, 0.6);
		platforms.add(cube);
		pbPlat.add(App->physics->AddBody(*cube, 0));
		checkPlat.add(pbPlat.getLast()->data);
	}
	//CHECKPOINTS
	{
		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(-110, 10 + platOffset, -86);
		cube->SetRotation(90, { 0,1,0 });
		checkpoints.add(cube);
		temp = App->physics->AddBody(*cube, 0);
		temp->ctype = ColliderType::CHECKPOINT;
		pbPlat.add(temp);
		pbPlat.getLast()->data->isSensor();
	}


	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
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
	
	//ground->Render();

	for (p2List_item<Cube*>* cube = platforms.getFirst(); cube; cube = cube->next)
	{
		cube->data->Render();
	}

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

