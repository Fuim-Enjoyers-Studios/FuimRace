#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 0.5f, 4);
	car.chassis_offset.Set(0, 1, 0);

	car.chassis2_size.Set(1.5f, 0.35f, 0.5f);
	car.chassis2_offset.Set(0, 1, car.chassis_size.z / 2 + car.chassis2_size.z / 2);

	/*car.cabin_size.Set(2,1.2f,1.5f);
	car.cabin_offset.Set(0, car.chassis_offset.y+car.chassis_size.y/2 + car.cabin_size.y / 2, -1.25f);*/

	car.cabin_size.Set(1, 0.5f, 1.5f);
	car.cabin_offset.Set(0, car.chassis_offset.y + car.chassis_size.y / 2 + car.cabin_size.y / 2, 0);

	car.front_size.Set(0.85f, 0.25f, 1);
	car.front_offset.Set(car.chassis_offset.x, car.chassis_offset.y, car.chassis_size.z / 2 + car.chassis2_size.z + car.front_size.z / 2);

	car.front_horitzontal_size.Set(2, 0.125f, 0.5f);
	car.front_horitzontal_offset.Set(car.chassis_offset.x, car.chassis_offset.y, car.chassis_size.z / 2 + car.chassis2_size.z + car.front_size.z);

	/*car.glass_size.Set(2, 0.25f, 1.75f);
	car.glass_offset.Set(car.chassis_offset.x, 1.75f, 0);*/

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.3f;
	float wheel_width = 0.3f;
	float suspensionRestLength = 0.6f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->ctype = ColliderType::PLAYER;
	vec3 cam = App->scene_intro->checkpoints.at(0).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(0).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);

	lastCheckpoint = 0;
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (vehicle->body->getCenterOfMassPosition().getY() < App->scene_intro->platOffset - 5) respawn = true;
	if (respawn) Teleport(lastCheckpoint);

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	/*char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());*/
	const char* title = "FuimRace";
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Teleport(int num)
{
	respawn = false;
	vehicle->body->setLinearVelocity({ 0,0,0 });
	vehicle->body->setAngularVelocity({ 0,0,0 });
	vec3 cam = App->scene_intro->checkpoints.at(num).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(num).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);
}



