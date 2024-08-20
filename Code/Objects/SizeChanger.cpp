#include "SizeChanger.h"
#include <Objects/Components/MeshComponent.h>
#include <Objects/Components/PhysicsComponent.h>
#include <Engine/Subsystem/Scene.h>
#include <Objects/Player.h>
#include <Objects/ParticleObject.h>

void SizeChanger::Begin()
{
	auto Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("Teleporter");
	Mesh->RelativeTransform.Scale = 2;
	Mesh->CastStaticShadow = false;

	auto Collider = new PhysicsComponent();
	Attach(Collider);
	Collider->CreateBox(Transform(0, 0, Vector3(4, 0.1f, 4)), Physics::MotionType::Static, Physics::Layer::Static);

	AddEditorProperty(Property("Size", NativeType::Int, &TargetSize));
#if EDITOR
	return;
#endif

	SignUIBox = (new UIBackground(UIBox::Orientation::Horizontal, 0, 0, 0))
		->SetOpacity(0.75f);
	SignText = new UIText(1, 1, "", GameUI::GetFont());
	SignUIBox->AddChild(SignText
		->SetWrapEnabled(true, 0.75f, UIBox::SizeMode::AspectRelative)
		->SetPadding(0.01f));
	SignUIBox
		->SetBorder(UIBox::BorderType::Rounded, 0.5f);
}

void SizeChanger::Update()
{
#if EDITOR
	return;
#endif

	Vector3 ScreenPosition = Player::Current->PlayerCamera->WorldToScreenPos(GetTransform().Position);

	if (Vector3::Distance(Player::Current->PlayerCamera->GetWorldTransform().Position, GetTransform().Position) < 50
		&& ScreenPosition.Z > 0)
	{
		SignUIBox->SetPosition(Vector2(ScreenPosition.X, ScreenPosition.Y) - SignUIBox->GetUsedSize() / 2);
		SignUIBox->IsVisible = true;
	}
	else
	{
		SignUIBox->IsVisible = false;
	}

	switch (TargetSize)
	{
	case 0:
		SignText->SetText("Jump for small size");
		break;
	case 1:
		SignText->SetText("Jump for medium size");
		break;
	case 2:
		SignText->SetText("Jump for large size");
		break;
	default:
		break;
	}

}

void SizeChanger::Use()
{
	Player::Current->UI->StartTransition([this]() {
		Player::Current->Size = Player::RobotSize(TargetSize);
		Player::Current->HasControl = true;
		Player::Current->UpdateAnimations();
		});
	Objects::SpawnObject<ParticleObject>(GetTransform())->LoadParticle("FuelCanPickup");
}
