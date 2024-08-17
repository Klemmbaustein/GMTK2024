#include "WaterPlane.h"

void WaterPlane::Begin()
{
	Attach(WaterMesh);
	WaterMesh->Load("WaterPlane");

	Attach(WaterCollider);
	WaterCollider->CreateBox(Transform(0, 0, Vector3(2, 0.25f, 2)), Physics::MotionType::Static, Physics::Layer::Static);
}
