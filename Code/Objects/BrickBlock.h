#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/BrickBlock.h>

class BrickBlock : public SceneObject
{
public:
	BRICKBLOCK_GENERATED("Level");

	void Begin() override;
	void Update() override;
	void Break();
};