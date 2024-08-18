#include "PlayerShadow.h"
#include <Engine/Log.h>
#include <Engine/Input.h>

void PlayerShadow::Begin()
{
	Attach(Mesh);
	Mesh->Load("PlayerShadow");
}

void PlayerShadow::Update()
{
	auto Cast = Physics::RayCast(
		Parent->GetTransform().Position,
		Parent->GetTransform().Position - Vector3(0, 1000, 0),
		Physics::Layer::Dynamic,
		{Parent}
	);

	if (Cast.Hit)
	{
		GetTransform().Position = Cast.ImpactPoint + Vector3(0, 0.02f, 0);
		GetTransform().Rotation = Vector3::LookAtFunctionY(0, Cast.Normal);
	}
	GetTransform().Scale = Parent->GetScaleValue() * 2;

	if (Input::IsKeyDown(Input::Key::c))
	{
		Parent->GetTransform().Position = GetTransform().Position;
	}
}
