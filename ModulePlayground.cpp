#include "Globals.h"
#include "Application.h"
#include "ModulePlayground.h"
#include "Primitive.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"

ModulePlayground::ModulePlayground(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayground::~ModulePlayground()
{}

// Load assets
bool ModulePlayground::Start()
{
	CreatePlayground();
	bool ret = true;
	return ret;
}

// Load assets
bool ModulePlayground::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModulePlayground::Update(float dt)
{
	p2List_item<Cube*>* c = scenaryCubes.getFirst();

	while (c != NULL) {
		c->data->Render();
		c = c->next;
	}

	
	return UPDATE_CONTINUE;
}

void ModulePlayground::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModulePlayground::CreatePlayground() {

	// Colors
	Color Red(1.0, 0.0, 0.0);
	Color Green(0.0, 1.0, 0.0);
	Color Blue(0.0, 0.0, 1.0);
	Color Water = Blue;
	Color Dirt(0.4, 0.2, 0.0);

	CreateScenarioCube(vec3(10, 0.5f, 80), vec3(0, 0, 0));

	CreateScenarioCube(vec3(11.5f, 0.5f, 21.2f), vec3(6.7f, 0,43.5f),45);

	CreateScenarioCube(vec3(10, 0.5f, 40), vec3(30, 0, 50),90);

	CreateScenarioCube(vec3(11.5f, 0.5f, 21.2f), vec3(53.3f, 0, 43.5f), -45);

	CreateScenarioCube(vec3(10, 0.5f, 80), vec3(60, 0, 0));

	CreateScenarioCube(vec3(11.5f, 0.5f, 21.2f), vec3(6.7f, 0, -43.5f), -45);

	CreateScenarioCube(vec3(10, 0.5f, 40), vec3(30, 0, -50), 90);

	CreateScenarioCube(vec3(11.5f, 0.5f, 21.2f), vec3(53.3f, 0, -43.5f), 45);


}

void ModulePlayground::CreateScenarioCube(vec3 size, vec3 pos, float angle, Color color, Ctype ctype) {

	Cube* cube = new Cube(size.x, size.y, size.z);

	cube->SetPos(pos.x, pos.y, pos.z);
	cube->SetRotation(angle, vec3(0, 1, 0));

	PhysBody3D* PhysBody = App->physics->AddBody(*cube, 0);
	PhysBody->SetPos(pos.x, pos.y, pos.z);
	PhysBody->ctype = ctype;	

	PhysBody->SetTransform(cube->transform.M);

	cube->color.Set(color.r, color.g, color.b, color.a);

	scenaryCubes.add(cube);
}
