#pragma once
#include <GENERATED/Player.h>
#include <Objects/SceneObject.h>
#include <Objects/WallSlideEffects.h>
#include <Objects/Components/MoveComponent.h>
#include <Objects/Components/CameraComponent.h>
#include <Objects/Components/MeshComponent.h>
#include <Objects/Components/PhysicsComponent.h>
#include <Engine/Gamepad.h>
	
class Player : public SceneObject
{
public:
	PLAYER_GENERATED("Game");

	void Begin() override;
	void Update() override;

	void GamepadInput(Input::Gamepad& Gamepad);

	MoveComponent* Movement = new MoveComponent();
	CameraComponent* PlayerCamera = new CameraComponent();
	MeshComponent* PlayerMesh = new MeshComponent();
	PhysicsComponent* CameraCollider = new PhysicsComponent();

	WallSlideEffects* SlideEffects = nullptr;

	void UpdateAnimations();

	float AnimationUpdateTimer = 0;

	enum class RobotSize
	{
		Small,
		Medium,
		Large
	};
	RobotSize Size = RobotSize::Medium;
	Vector3 LastSpawnPoint;

	float GetScaleValue();

private:

	bool CanWallJump();
	void SpawnJumpParticles();
	void ApplyScale();

	bool HoldingJump = false;
	bool HoldingGamepadJump = false;
	bool IsJumping = false;
	bool HasDoubleJumped = false;
	float DoubleJumpTimer = 0;
};