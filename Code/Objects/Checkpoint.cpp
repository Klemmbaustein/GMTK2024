#include "Checkpoint.h"
#include <Objects/Player.h>

void Checkpoint::Begin()
{
	Attach(NormalCheckpoint);
	NormalCheckpoint->Load("Checkpoint");

	Attach(ActivatedCheckpoint);
	ActivatedCheckpoint->Load("ActivatedCheckpoint");
	ActivatedCheckpoint->SetVisibility(false);

	ActivatedCheckpoint->RelativeTransform.Position.Y = 0.5f;
	NormalCheckpoint->RelativeTransform.Position = ActivatedCheckpoint->RelativeTransform.Position;
}

void Checkpoint::Update()
{
#if EDITOR
	return;
#endif

	bool Active = IsActive();

	if (Vector3::Distance(Player::Current->GetTransform().Position, GetTransform().Position) < 10)
	{
		Player::Current->LastSpawnPoint = GetTransform().Position;
	}


	ActivatedCheckpoint->SetVisibility(Active);
	NormalCheckpoint->SetVisibility(!Active);
}

bool Checkpoint::IsActive()
{
	return Player::Current->LastSpawnPoint == GetTransform().Position;
}
