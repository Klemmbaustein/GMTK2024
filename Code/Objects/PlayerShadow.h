#pragma once
#include <GENERATED/PlayerShadow.h>
#include <Objects/SceneObject.h>
#include <Objects/Components/MeshComponent.h>
#include <Objects/Player.h>

class PlayerShadow : public SceneObject
{
public:
	PLAYERSHADOW_GENERATED("Game/Misc");

	MeshComponent* Mesh = new MeshComponent();
	Player* Parent = nullptr;

	void Begin() override;
	void Update() override;
};