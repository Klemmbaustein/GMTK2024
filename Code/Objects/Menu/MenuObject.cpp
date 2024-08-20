#include "MenuObject.h"
#include <Objects/Components/CameraComponent.h>
#include <Engine/Input.h>
#include <UI/MenuUI.h>

void MenuObject::Begin()
{
	auto* Camera = new CameraComponent();
	Attach(Camera);
	Camera->Use();
	Input::CursorVisible = true;
	UICanvas::CreateNewCanvas<MenuUI>();
}
