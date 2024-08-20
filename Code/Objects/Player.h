#pragma once
#include <GENERATED/Player.h>
#include <Objects/SceneObject.h>
#include <Objects/WallSlideEffects.h>
#include <Objects/Components/MoveComponent.h>
#include <Objects/Components/CameraComponent.h>
#include <Objects/Components/MeshComponent.h>
#include <Objects/Components/PhysicsComponent.h>
#include <Engine/Gamepad.h>
#include <Engine/Subsystem/Sound.h>
#include <UI/GameUI.h>
	
class Player : public SceneObject
{
public:
	PLAYER_GENERATED("Game");

	void Begin() override;
	void Update() override;

	void GamepadInput(Input::Gamepad& Gamepad);

	MoveComponent* Movement = new MoveComponent();
	CameraComponent* PlayerCamera = new CameraComponent();
	PhysicsComponent* CameraCollider = new PhysicsComponent();

	WallSlideEffects* SlideEffects = nullptr;

	void UpdateAnimations();


	enum class RobotSize
	{
		Small,
		Medium,
		Large
	};
	static RobotSize Size;
	Vector3 LastSpawnPoint;

	float GetScaleValue();

	static Player* Current;

	static uint64_t Score;

	GameUI* UI = UICanvas::CreateNewCanvas<GameUI>();
	bool PlayerVisible = false;
	bool HasControl = true;

	Sound::SoundSource JetpackSound;

private:

	struct Animation
	{
		float Scale;
		float Speed;
		std::vector<std::string> MeshFiles;
		std::vector<MeshComponent*> MeshComponents;
	};
	ParticleComponent* SmallJumpThrust = new ParticleComponent();
	ParticleComponent* MediumHoverEffect = new ParticleComponent();

	Vector3 AnimationFacing;
	float DoubleJumpTimer = 0;

	static std::vector<std::vector<Animation>> PlayerAnimations;
	size_t CurrentAnimation = SIZE_MAX;
	size_t AnimationFrame = 0;
	float AnimationUpdateTimer = 0;
	float GlideTimer = 0;

	void CommonMovementLogic();
	size_t GetActiveAnimationSmall();
	size_t GetActiveAnimationMedium();

	void SmallRobotLogic();
	void MediumRobotLogic();
	void LargeRobotLogic();

	bool CanWallJump() const;
	void SpawnJumpParticles();
	void ApplyScale();

	void DoubleJump();
	void Glide();
	void Land();

	void LoadAnimations();

	bool HoldingJump = false;
	bool HoldingGamepadJump = false;
	bool IsJumping = false;
	bool HasDoubleJumped = false;
	bool Gliding = false;
	bool Landing = false;
};