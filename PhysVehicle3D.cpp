#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btVector3 offset6(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset6 = offset6.rotate(q.getAxis(), q.getAngle());

	chassis2.transform.M[12] += offset6.getX();
	chassis2.transform.M[13] += offset6.getY();
	chassis2.transform.M[14] += offset6.getZ();

	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 offset2(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += offset2.getX();
	cabin.transform.M[13] += offset2.getY();
	cabin.transform.M[14] += offset2.getZ();

	Cube front(info.front_size.x, info.front_size.y, info.front_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front.transform);
	btVector3 offset3(info.front_offset.x, info.front_offset.y, info.front_offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	front.transform.M[12] += offset3.getX();
	front.transform.M[13] += offset3.getY();
	front.transform.M[14] += offset3.getZ();

	Cube front_horitzontal(info.front_horitzontal_size.x, info.front_horitzontal_size.y, info.front_horitzontal_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front_horitzontal.transform);
	btVector3 offset4(info.front_horitzontal_offset.x, info.front_horitzontal_offset.y, info.front_horitzontal_offset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	front_horitzontal.transform.M[12] += offset4.getX();
	front_horitzontal.transform.M[13] += offset4.getY();
	front_horitzontal.transform.M[14] += offset4.getZ();

	Cube glass(info.glass_size.x, info.glass_size.y, info.glass_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&glass.transform);
	btVector3 offset5(info.glass_offset.x, info.glass_offset.y, info.glass_offset.z);
	offset5 = offset5.rotate(q.getAxis(), q.getAngle());

	glass.transform.M[12] += offset5.getX();
	glass.transform.M[13] += offset5.getY();
	glass.transform.M[14] += offset5.getZ();

	
	//const vec3 axis(1, 0, 0);
	//glass.transform.rotate(45, vehicle->getRightAxis());
	


	chassis.Render();
	chassis2.Render();
	cabin.Render();
	front.Render();
	front_horitzontal.Render();
	glass.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}