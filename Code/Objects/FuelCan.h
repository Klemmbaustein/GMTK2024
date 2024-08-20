#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/FuelCan.h>

class FuelCan : public SceneObject
{
public:
	FUELCAN_GENERATED("Level");

	void Begin() override;
	void Update() override;
};