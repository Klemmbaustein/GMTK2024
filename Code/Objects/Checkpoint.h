#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/Checkpoint.h>
#include <Objects/Components/MeshComponent.h>

class Checkpoint : public SceneObject
{
public:
	CHECKPOINT_GENERATED("Level");

	void Begin() override;
	void Update() override;

	bool IsActive();
private:
	MeshComponent* NormalCheckpoint = new MeshComponent();
	MeshComponent* ActivatedCheckpoint = new MeshComponent();
};