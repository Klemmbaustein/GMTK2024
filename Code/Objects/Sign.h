#pragma once
#include <GENERATED/Sign.h>
#include <Objects/SceneObject.h>
#include <UI/UIText.h>

class Sign : public SceneObject
{
public:
	SIGN_GENERATED("Level");

	void Begin() override;
	void Update() override;
	void Destroy() override;

	UIBox* SignUIBox = nullptr;
	UIText* SignText = nullptr;

	void OnPropertySet() override;

	std::string SignString;
};