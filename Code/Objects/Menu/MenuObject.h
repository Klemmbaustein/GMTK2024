#pragma once
#include <Objects/SceneObject.h>
#include <GENERATED/MenuObject.h>

class MenuObject : public SceneObject
{
public:
	MENUOBJECT_GENERATED("Game");

	void Begin() override;
};