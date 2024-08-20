#include "Fan.h"
#include <Objects/Components/MeshComponent.h>
#include <Engine/Stats.h>
#include <Objects/Player.h>

void Fan::Begin()
{
	MeshComponent* Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("Fan");
	Mesh->RelativeTransform.Scale = 2;
}

void Fan::Update()
{
#if EDITOR
	return;
#endif

	Vector3 PlayerPosition = Player::Current->GetTransform().Position;

	if (Vector3::NearlyEqual(PlayerPosition * Vector3(1, 0, 1), GetTransform().Position * Vector3(1, 0, 1), 6))
	{
		float PositionDifference = PlayerPosition.Y - GetTransform().Position.Y;
		if (PositionDifference < 0)
		{
			return;
		}
		Player::Current->Movement->SetVelocity(Player::Current->Movement->GetVelocity() + Vector3(0, Stats::DeltaTime * (80 - PositionDifference) * 2, 0));
		float OldHeight = Player::Current->Movement->JumpHeight;
		Player::Current->Movement->JumpHeight = 2;
		Player::Current->Movement->Jump();
		Player::Current->Movement->JumpHeight = OldHeight;
	}
}

