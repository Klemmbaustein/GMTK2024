#include "WaterPlane.h"
#include <Objects/Player.h>

void WaterPlane::Begin()
{
	Attach(WaterMesh);
	WaterMesh->Load("WaterPlane");
	WaterMesh->SetUniform("u_dirtiness", NativeType::Float, std::to_string(float(Player::Score) / 30.0f), 0);

	Attach(WaterCollider);
	WaterCollider->CreateBox(Transform(0, 0, Vector3(2, 0.25f, 2)), Physics::MotionType::Static, Physics::Layer::Static);
}
