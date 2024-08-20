#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/Fan.h>

class Fan : public SceneObject
{
public:
	FAN_GENERATED("Level");

	void Begin() override;
	void Update() override;
};