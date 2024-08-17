#pragma once
#include <GENERATED/WaterPlane.h>
#include <Objects/SceneObject.h>
#include <Objects/Components/MeshComponent.h>
#include <Objects/Components/PhysicsComponent.h>

class WaterPlane : public SceneObject
{
public:
	WATERPLANE_GENERATED("Level");

	void Begin() override;
	MeshComponent* WaterMesh = new MeshComponent();
	PhysicsComponent* WaterCollider = new PhysicsComponent();
};