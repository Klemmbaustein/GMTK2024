#include "Player.h"
#include <Engine/Input.h>
#include <Engine/Stats.h>
#include <Math/Physics/Physics.h>
#include <Engine/Log.h>
#include <algorithm>
#include <Objects/ParticleObject.h>
#include <Objects/PlayerShadow.h>
#include <Rendering/Camera/CameraShake.h>
#include "WaterPlane.h"

void Player::Begin()
{
	float Scale = GetScaleValue();
	Movement->ColliderSize = Scale;
	Attach(Movement);
	Movement->AirAccelMultiplier = 0.25f;

	Attach(PlayerCamera);
	PlayerCamera->RelativeTransform.Position.Y = 1;
	PlayerCamera->Use();

	Attach(PlayerMesh);
	PlayerMesh->Load("Cube");
	PlayerMesh->CastStaticShadow = false;

	Attach(CameraCollider);
	CameraCollider->SetActive(false);
	CameraCollider->CreateSphere(Transform(0, 0, 1), Physics::MotionType::Static, Physics::Layer::Static);
#if EDITOR
	return;
#endif

	Objects::SpawnObject<PlayerShadow>(Transform())->Parent = this;

	SlideEffects = Objects::SpawnObject<WallSlideEffects>(GetTransform());
	LastSpawnPoint = GetTransform().Position;
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
	

	HoldingGamepadJump = false;
	for (auto& i : Input::Gamepads)
	{
		GamepadInput(i.second);
	}

	PlayerCamera->RelativeTransform.Rotation.X = std::clamp(PlayerCamera->RelativeTransform.Rotation.X, -80.0f, 25.0f);

	Vector3 CameraPosition = Vector3(0, Scale * 3.0f, 0) + GetTransform().Position;
	Vector3 CameraDirection = Vector3::GetForwardVector(PlayerCamera->RelativeTransform.Rotation) * -30.0f * Scale;
	Vector3 CameraEndPosition = CameraPosition + CameraDirection;

	Physics::HitResult CameraHit = CameraCollider->ShapeCast(
		Transform(CameraPosition, 0, 0.5f),
		CameraEndPosition,
		Physics::Layer::Static,
		{this}
	);

	if (CameraHit.Hit)
	{
		PlayerCamera->RelativeTransform.Position = CameraPosition + CameraDirection * CameraHit.Distance - GetTransform().Position;
	}
	else
	{
		PlayerCamera->RelativeTransform.Position = CameraEndPosition - GetTransform().Position;
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
	if (HoldingJump || HoldingGamepadJump)
	{
		if (Movement->GetIsOnGround())
		{
			SpawnJumpParticles();
			DoubleJumpTimer = 0.25f;
			Movement->Jump();
			IsJumping = true;
			CameraShake::PlayDefaultCameraShake(1);
		}
		else if (!IsJumping && !HasDoubleJumped && DoubleJumpTimer <= 0 && !CanWallJump())
		{
			Vector3 Velocity = Movement->GetVelocity();
			Movement->SetVelocity(Vector3(Velocity.X, 40 * Scale, Velocity.Z));
			SpawnJumpParticles();
			HasDoubleJumped = true;
			IsJumping = true;
			CameraShake::PlayDefaultCameraShake(1);
		}
	}

	if (Movement->GetIsOnGround())
	{
		if (Movement->StoodOn && typeid(*Movement->StoodOn->GetParent()) == typeid(WaterPlane))
		{
			GetTransform().Position = LastSpawnPoint;
			Movement->SetVelocity(0);
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
			vel = (Movement->LastHitNormal + Vector3(0, 1, 0)) * 30 * Scale;
			CameraShake::PlayDefaultCameraShake(1);
			SlideEffects->WallJump();
		}
		else
		{
			vel -= Movement->LastHitNormal * Stats::DeltaTime * 10;
		}
		HasDoubleJumped = false;
		Movement->SetVelocity(Vector3(vel.X, std::max(-3.0f, vel.Y), vel.Z));
	}
	else
	{
		SlideEffects->Stop();
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

	Movement->Gravity = (HoldingJump || HoldingGamepadJump ? 50 : 100) * Scale;

	if (Input::IsKeyDown(Input::Key::k1))
	{
		Size = RobotSize::Small;
	}
	if (Input::IsKeyDown(Input::Key::k2))
	{
		Size = RobotSize::Medium;
	}
	if (Input::IsKeyDown(Input::Key::k3))
	{
		Size = RobotSize::Large;
	}
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

void Player::UpdateAnimations()
{
	AnimationUpdateTimer += Stats::DeltaTime;
	if (AnimationUpdateTimer < 0.1f)
	{
		return;
	}
	AnimationUpdateTimer = 0;
	Vector3 PlayerDirection = Movement->GetVelocity() * Vector3(1, 0, 1);
	PlayerMesh->RelativeTransform.Rotation = Vector3::LookAtFunction(0, PlayerDirection);
}

float Player::GetScaleValue()
{
	switch (Size)
	{
	case Player::RobotSize::Small:
		return 3;
	case Player::RobotSize::Large:
		return 0.5f;
	default:
		return 1;
	}
}

bool Player::CanWallJump()
{
	return !Movement->GetIsOnGround() && !Movement->LastMoveSuccessful && Movement->GetVelocity().Y > -20;
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
	Movement->Deceleration = 125 * Scale;
	Movement->Acceleration = 125 * Scale;
	Movement->JumpHeight = 30 * Scale;
	Movement->ColliderSize = Vector2(Scale * 2, Scale);
	Movement->MaxSpeed = 35 * Scale;
	PlayerMesh->RelativeTransform.Scale = Vector3(Scale, std::powf(Scale, 1.5f), Scale);
}
