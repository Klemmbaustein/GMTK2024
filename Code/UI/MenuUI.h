#pragma once
#include <UI/UICanvas.h>
#include <UI/UIButton.h>
#include <Engine/Gamepad.h>

class MenuUI : public UICanvas
{
public:
	MenuUI();

	void Tick() override;
	bool Active = true;

	void GamepadInput(const Input::Gamepad& Target);

	int SelectedOption = 0;
	std::vector<UIButton*> Buttons;

	void OnButtonClicked(int Index) override;
};