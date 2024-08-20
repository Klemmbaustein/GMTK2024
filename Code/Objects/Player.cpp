#include "Player.h"
#include <Engine/Input.h>
#include <Engine/Stats.h>
#include <Math/Physics/Physics.h>
#include <algorithm>
#include <Objects/ParticleObject.h>
#include <Objects/PlayerShadow.h>
#include <Objects/BrickBlock.h>
#include <Rendering/Camera/CameraShake.h>
#include "WaterPlane.h"
#include <Objects/Teleporter.h>
#include <Objects/SizeChanger.h>
#include <Engine/Log.h>
#include <Sounds.h>
#include <Engine/EngineRandom.h>

Player* Player::Current = nullptr;
uint64_t Player::Score = 0;
Player::RobotSize Player::Size = Player::RobotSize::Medium;
void Player::Begin()
{
	float Scale = GetScaleValue();
	Attach(Movement);
	Movement->AirAccelMultiplier = 0.25f;

	Attach(PlayerCamera);
	PlayerCamera->RelativeTransform.Position.Y = 1;
	PlayerCamera->Use();

	Attach(CameraCollider);
	CameraCollider->SetActive(false);
	CameraCollider->CreateSphere(Transform(0, 0, 1), Physics::MotionType::Static, Physics::Layer::Static);
	Attach(SmallJumpThrust);
	Attach(MediumHoverEffect);
#if EDITOR
	return;
#endif

	LoadAnimations();

	Objects::SpawnObject<PlayerShadow>(Transform())->Parent = this;

	SlideEffects = Objects::SpawnObject<WallSlideEffects>(GetTransform());
	LastSpawnPoint = GetTransform().Position;
	SmallJumpThrust->LoadParticle("Thruster");
	SmallJumpThrust->SetActive(false);

	MediumHoverEffect->LoadParticle("Hover");
	MediumHoverEffect->RelativeTransform.Position.Y = -2.5f;
	MediumHoverEffect->SetActive(false);

	Sounds::Load();
	JetpackSound = Sound::PlaySound2D(Sounds::Jetpack, 1, 0, true);
	Current = this;
}

void Player::Update()
{
#if EDITOR
	return;
#endif
	float Scale = GetScaleValue();

	PlayerCamera->RelativeTransform.Rotation += Vector3(Input::MouseMovement.Y, Input::MouseMovement.X, 0);

	float OnWallShake = CanWallJump() ? sin(Stats::Time * 10) : 0;
	PlayerCamera->RelativeTransform.Rotation.Z = OnWallShake;

	Vector3 CameraRotation = Vector3(0, PlayerCamera->RelativeTransform.Rotation.Y, 0);

	PlayerCamera->RelativeTransform.Rotation.X = std::clamp(PlayerCamera->RelativeTransform.Rotation.X, -80.0f, 25.0f);

	Vector3 CameraPosition = Vector3(0, Scale * 3.0f, 0) + GetTransform().Position;
	Vector3 CameraDirection = Vector3::GetForwardVector(PlayerCamera->RelativeTransform.Rotation) * -30.0f * Scale;
	Vector3 CameraEndPosition = CameraPosition + CameraDirection;

	Physics::HitResult CameraHit = CameraCollider->ShapeCast(
		Transform(CameraPosition, 0, 0.5f),
		CameraEndPosition,
		Physics::Layer::Static,
		{ this }
	);

	if (CameraHit.Hit)
	{
		PlayerCamera->RelativeTransform.Position = CameraPosition + CameraDirection * CameraHit.Distance - GetTransform().Position;
	}
	else
	{
		PlayerCamera->RelativeTransform.Position = CameraEndPosition - GetTransform().Position;
	}

	if (!HasControl)
	{
		return;
	}

	HoldingGamepadJump = false;
	for (auto& i : Input::Gamepads)
	{
		GamepadInput(i.second);
	}

	if (Input::IsKeyDown(Input::Key::w))
	{
		Movement->AddMovementInput(Vector3::GetForwardVector(CameraRotation));
	}
	if (Input::IsKeyDown(Input::Key::s))
	{
		Movement->AddMovementInput(-Vector3::GetForwardVector(CameraRotation));
	}
	if (Input::IsKeyDown(Input::Key::d))
	{
		Movement->AddMovementInput(Vector3::GetRightVector(CameraRotation));
	}
	if (Input::IsKeyDown(Input::Key::a))
	{
		Movement->AddMovementInput(-Vector3::GetRightVector(CameraRotation));
	}

	HoldingJump = Input::IsKeyDown(Input::Key::SPACE);

	CommonMovementLogic();

	switch (Size)
	{
	case Player::RobotSize::Small:
		SmallRobotLogic();
		break;
	case Player::RobotSize::Medium:
		MediumRobotLogic();
		break;
	case Player::RobotSize::Large:
		LargeRobotLogic();
		break;
	default:
		break;
	}

	if (HoldingJump || HoldingGamepadJump)
	{
		IsJumping = true;
	}
	else
	{
		IsJumping = false;
	}

	DoubleJumpTimer -= Stats::DeltaTime;

	ApplyScale();
	UpdateAnimations();

	Movement->Gravity = Landing ? 600 : (HoldingJump || HoldingGamepadJump ? 50 : 100) * Scale;
}

void Player::GamepadInput(Input::Gamepad& Gamepad)
{
	Vector3 CameraRotation = Vector3(0, PlayerCamera->RelativeTransform.Rotation.Y, 0);
	
	if (Gamepad.Buttons[Input::GAMEPAD_A])
	{
		HoldingGamepadJump = true;
	}

	Movement->AddMovementInput(Vector3::GetForwardVector(CameraRotation) * Gamepad.LeftStickPosition.Y * 2);
	Movement->AddMovementInput(Vector3::GetRightVector(CameraRotation) * Gamepad.LeftStickPosition.X * 2);

	PlayerCamera->RelativeTransform.Rotation += Vector3(Gamepad.RightStickPosition.Y, Gamepad.RightStickPosition.X, 0) * 200 * Stats::DeltaTime;
}

float Player::GetScaleValue()
{
	switch (Size)
	{
	case Player::RobotSize::Large:
		return 1.4f;
	case Player::RobotSize::Small:
		return 0.5f;
	default:
		return 1;
	}
}

void Player::UpdateAnimations()
{
	AnimationUpdateTimer += Stats::DeltaTime;

	auto& CurrentAnims = PlayerAnimations[std::min(int(Size), int(PlayerAnimations.size() - 1))];

	size_t NewAnimation = GetActiveAnimationSmall();
	if ((AnimationUpdateTimer < CurrentAnims[CurrentAnimation].Speed && NewAnimation == CurrentAnimation) && !PlayerVisible)
	{
		return;
	}

	if (CanWallJump())
	{
		AnimationFacing = Movement->LastHitNormal;
	}
	else if ((Movement->GetVelocity() * Vector3(1, 0, 1)).Length() >= 5)
	{
		AnimationFacing = Movement->GetVelocity();
	}
	AnimationFacing.Y = 0;
	AnimationFacing = AnimationFacing.Normalize();

	CurrentAnimation = NewAnimation;

	if (CurrentAnimation == 1 && AnimationFrame % 2 == 0 && Size != RobotSize::Large)
	{
		Sound::PlaySound2D(Sounds::WallJump, Random::GetRandomFloat(0.9f, 1.1f), 0.3f);
	}
	AnimationFrame++;
	if (AnimationFrame >= CurrentAnims[CurrentAnimation].MeshComponents.size())
	{
		AnimationFrame = 0;
	}

	for (auto& i : PlayerAnimations)
	{
		for (auto& j : i)
		{
			for (auto& k : j.MeshComponents)
			{
				k->SetVisibility(false);
			}
		}
	}

	if (PlayerVisible)
		return;

	// Weird rotation hack because I exported the models with the wrong rotation.
	Vector3 Rotation;
	if (Size != RobotSize::Large)
	{
		Rotation = Vector3::LookAtFunction(0, Vector3(AnimationFacing.Z, AnimationFacing.Y, -AnimationFacing.X));
	}
	else
	{
		Rotation = Vector3::LookAtFunction(0, -AnimationFacing);
	}
	for (MeshComponent* m : CurrentAnims[CurrentAnimation].MeshComponents)
	{
		m->SetVisibility(CurrentAnims[CurrentAnimation].MeshComponents[AnimationFrame] == m);
		m->RelativeTransform.Rotation = Rotation;
	}
		
	AnimationUpdateTimer = 0;
	Vector3 PlayerDirection = Movement->GetVelocity() * Vector3(1, 0, 1);
}

void Player::CommonMovementLogic()
{
	if ((HoldingJump || HoldingGamepadJump) && !Gliding && !IsJumping)
	{
		if (Movement->GetIsOnGround())
		{
			if (Movement->StoodOn && Movement->StoodOn->GetParent()->GetObjectDescription().ID == Teleporter::GetID())
			{
				Teleporter* tp = static_cast<Teleporter*>(Movement->StoodOn->GetParent());
				tp->Use();
				HasControl = false;
				return;
			}
			if (Movement->StoodOn && Movement->StoodOn->GetParent()->GetObjectDescription().ID == SizeChanger::GetID() && !UI->InTransition())
			{
				SizeChanger* tp = static_cast<SizeChanger*>(Movement->StoodOn->GetParent());
				tp->Use();
				HasControl = false;
				return;
			}
			SpawnJumpParticles();
			DoubleJumpTimer = 0.25f;
			Movement->Jump();
			IsJumping = true;
			CameraShake::StopAllCameraShake();
			Sound::PlaySound2D(Sounds::Jump, Random::GetRandomFloat(0.9f, 1.1f), 0.5f);
			CameraShake::PlayDefaultCameraShake(1);
		}
		else if (!IsJumping && !HasDoubleJumped && DoubleJumpTimer <= 0 && !CanWallJump())
		{
			switch (Size)
			{
			case Player::RobotSize::Small:
				Glide();
				break;
			case Player::RobotSize::Medium:
				DoubleJump();
				break;
			case Player::RobotSize::Large:
				Land();
				break;
			default:
				break;
			}
		}
	}

	if (Movement->GetIsOnGround() && !UI->InTransition())
	{
		if (Movement->StoodOn && typeid(*Movement->StoodOn->GetParent()) == typeid(WaterPlane))
		{
			Movement->SetVelocity(0);
			PlayerVisible = true;
			UpdateAnimations();
			MediumHoverEffect->SetActive(false);
			SmallJumpThrust->SetActive(false);
			Objects::SpawnObject<ParticleObject>(GetTransform())->LoadParticle("WaterImpact");
			Sound::PlaySound2D(Sounds::WaterSplash, Random::GetRandomFloat(0.9f, 1.1f), 0.5f);

			UI->StartTransition([this]() {
				GetTransform().Position = LastSpawnPoint + Vector3(0, GetScaleValue() * 25, 0);
				HasControl = true;
				PlayerVisible = false;
				});
			HasControl = false;
			return;
		}
		HasDoubleJumped = false;
	}

	if (CanWallJump())
	{
		SlideEffects->SetTransform(Transform(
			GetTransform().Position - Movement->LastHitNormal,
			Vector3::LookAtFunction(0, Movement->GetVelocity() * Vector3(1, 0, 1)),
			1));

		SlideEffects->StartSlide();
		Vector3 vel = Movement->GetVelocity();
		if ((HoldingJump || HoldingGamepadJump) && !IsJumping)
		{
			vel = (Movement->LastHitNormal + Vector3(0, 1, 0)) * 30 * std::powf(GetScaleValue(), 0.5f);
			CameraShake::PlayDefaultCameraShake(1);
			SlideEffects->WallJump();
			Sound::PlaySound2D(Sounds::WallJump, Random::GetRandomFloat(0.9f, 1.1f), 0.5f);
		}
		else
		{
			vel -= Movement->LastHitNormal * Stats::DeltaTime * 10;
		}
		Landing = false;
		HasDoubleJumped = false;
		GlideTimer = 2.0;
		Movement->SetVelocity(Vector3(vel.X, std::max(-3.0f, vel.Y), vel.Z));
	}
	else
	{
		SlideEffects->Stop();
	}

	Movement->AirAccelMultiplier = Size == RobotSize::Large ? 0.4f : (Gliding ? 0.5f : 0.25f);
}

size_t Player::GetActiveAnimationSmall()
{
	// Small Robot gliding
	if (Gliding)
		return 5;
	// Wall Slide
	if (CanWallJump())
		return 2;
	// Jumping (In air, moving up)
	if (!Movement->GetIsOnGround() && Movement->GetVelocity().Y > 10)
		return 3;
	// Falling (In air, moving down)
	else if (!Movement->GetIsOnGround())
		return 4;
	// Walking
	if (Movement->GetVelocity().Length() > 5 && Movement->GetIsOnGround())
		return 1;

	// Idle
	return 0;
}

size_t Player::GetActiveAnimationMedium()
{
	// Wall Slide
	if (CanWallJump())
		return 2;
	// Jumping (In air, moving up)
	if (!Movement->GetIsOnGround() && Movement->GetVelocity().Y > 10)
		return 3;
	// Falling (In air, moving down)
	else if (!Movement->GetIsOnGround())
		return 4;
	// Walking
	if (Movement->GetVelocity().Length() > 5 && Movement->GetIsOnGround())
		return 1;

	// Idle
	return 0;
}

void Player::SmallRobotLogic()
{
	MediumHoverEffect->SetActive(false);
	JetpackSound.SetVolume(Gliding ? 0.5f : 0);

	if (Movement->GetIsOnGround())
	{
		GlideTimer = 2.0;
		Gliding = false;
	}

	SmallJumpThrust->SetActive(Gliding);
	if (Gliding && !HoldingJump && !HoldingGamepadJump)
	{
		Gliding = false;
	}

	if (!Gliding)
	{
		return;
	}

	GlideTimer = std::max(GlideTimer - Stats::DeltaTime, -1.0f);

	Movement->SetVelocity(Movement->GetVelocity() * Vector3(1, 0, 1) + Vector3(0, GlideTimer * 5, 0));
	CameraShake::PlayDefaultCameraShake(Stats::DeltaTime * 5.0f);
}

void Player::MediumRobotLogic()
{
	MediumHoverEffect->SetActive(true);
	SmallJumpThrust->SetActive(false);
	Gliding = false;
}

void Player::LargeRobotLogic()
{
	SmallJumpThrust->SetActive(false);
	MediumHoverEffect->SetActive(false);

	if (Landing && Movement->GetIsOnGround())
	{
		Landing = false;
		CameraShake::PlayDefaultCameraShake(1);

		if (!Movement->StoodOn)
		{
			return;
		}

		auto Bricks = dynamic_cast<BrickBlock*>(Movement->StoodOn->GetParent());
		if (Bricks)
		{
			Objects::SpawnObject<ParticleObject>(Bricks->GetTransform())->LoadParticle("BreakBricks");
			Bricks->Break();
		}
		Sound::PlaySound2D(Sounds::Impact, Random::GetRandomFloat(0.9f, 1.1f), 0.5f);
		Movement->StoodOn = nullptr;
	}

	if (Landing && !HoldingJump && !HoldingGamepadJump)
	{
		Landing = false;
	}
}

bool Player::CanWallJump() const
{
	return !Movement->GetIsOnGround() && !Movement->LastMoveSuccessful && Movement->GetVelocity().Y > -40 && Size != RobotSize::Large && !Gliding;
}

void Player::SpawnJumpParticles()
{
	float Scale = GetScaleValue();
	Transform ParticleTransform = GetTransform();
	ParticleTransform.Position -= Vector3(0, std::powf(Scale, 1.5f) * 2, 0);
	Objects::SpawnObject<ParticleObject>(ParticleTransform)->LoadParticle("JumpDust");
}

void Player::ApplyScale()
{
	float Scale = GetScaleValue();
	Movement->Deceleration = Size != RobotSize::Large ? 125 * Scale : 50;
	Movement->Acceleration = Size != RobotSize::Large ? 125 * Scale : 100;
	Movement->JumpHeight = Size != RobotSize::Large ? 30 : 85;
	Movement->ColliderSize = Vector2(Scale * 2, Scale);
	Movement->MaxSpeed = 35 * std::pow(Scale, 0.75f);
}

void Player::DoubleJump()
{
	Vector3 Velocity = Movement->GetVelocity();
	Movement->SetVelocity(Vector3(Velocity.X, 40 * GetScaleValue(), Velocity.Z));
	SpawnJumpParticles();
	HasDoubleJumped = true;
	IsJumping = true;
	CameraShake::PlayDefaultCameraShake(1);
	Sound::PlaySound2D(Sounds::DoubleJump, 1, 0.5f);
}

void Player::Glide()
{
	CameraShake::PlayDefaultCameraShake(0.35f);
	Gliding = true;
	IsJumping = true;
}

void Player::Land()
{
	Landing = true;
}

void Player::LoadAnimations()
{
	for (auto& Anims : PlayerAnimations)
	{
		for (Animation& Anim : Anims)
		{
			Anim.MeshComponents.clear();
			for (const std::string& File : Anim.MeshFiles)
			{
				MeshComponent* NewComponent = new MeshComponent();
				Attach(NewComponent);
				NewComponent->Load(File);
				NewComponent->RelativeTransform.Scale = Anim.Scale;
				NewComponent->SetVisibility(false);
				Anim.MeshComponents.push_back(NewComponent);
			}
		}
	}
	UpdateAnimations();
}
