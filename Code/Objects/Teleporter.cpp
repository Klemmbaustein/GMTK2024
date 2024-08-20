#include "Teleporter.h"
#include <Objects/Components/MeshComponent.h>
#include <Objects/Components/PhysicsComponent.h>
#include <Engine/Subsystem/Scene.h>
#include <Objects/Player.h>
#include <Objects/ParticleObject.h>

void Teleporter::Begin()
{
	auto Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("Teleporter");
	Mesh->RelativeTransform.Scale = 2;
	Mesh->CastStaticShadow = false;

	auto Collider = new PhysicsComponent();
	Attach(Collider);
	Collider->CreateBox(Transform(0, 0, Vector3(4, 0.1f, 4)), Physics::MotionType::Static, Physics::Layer::Static);

	AddEditorProperty(Property("TargetScene", NativeType::String, &TargetScene));
}

void Teleporter::Update()
{
}

void Teleporter::Use()
{
	Player::Current->PlayerVisible = false;
	Player::Current->UpdateAnimations();
	Player::Current->UI->StartTransition([this]() {
		Scene::LoadNewScene(TargetScene);
		});
	Objects::SpawnObject<ParticleObject>(GetTransform())->LoadParticle("FuelCanPickup");
}
