#include "BrickBlock.h"
#include "Objects/Components/MeshComponent.h"
#include "Objects/Components/PhysicsComponent.h"

void BrickBlock::Begin()
{
	auto Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("BrickBlock");
	Mesh->RelativeTransform.Scale = 2;
	Mesh->CastStaticShadow = false;

	auto Collider = new PhysicsComponent();
	Attach(Collider);
	Collider->CreateBox(Transform(Mesh->GetBoundingBox().center, 0, Mesh->GetBoundingBox().extents * 4.0f), Physics::MotionType::Static, Physics::Layer::Static);
}

void BrickBlock::Update()
{
}

void BrickBlock::Break()
{
	while (Components.size())
	{
		Detach(Components[0]);
	}

	Objects::DestroyObject(this);
}
