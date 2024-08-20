#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/Teleporter.h>

class Teleporter : public SceneObject
{
public:
	TELEPORTER_GENERATED("Level");

	void Begin() override;
	void Update() override;

	void Use();

	std::string TargetScene;
};