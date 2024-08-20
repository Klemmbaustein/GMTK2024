#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/SizeChanger.h>
#include <UI/UIText.h>

class SizeChanger : public SceneObject
{
public:
	SIZECHANGER_GENERATED("Level");

	void Begin() override;
	void Update() override;

	void Use();

	int TargetSize = 0;

	UIBox* SignUIBox = nullptr;
	UIText* SignText = nullptr;

};