#include "MenuUI.h"
#include <UI/GameUI.h>
#include <UI/UIButton.h>
#include <Engine/Subsystem/Scene.h>
#include <Engine/AppWindow.h>
#include <Engine/Application.h>
#include <Engine/Input.h>
#include <Engine/Log.h>
#include <algorithm>
#include <UI/TextScreen.h>

bool UsedController = false;

static const std::vector<std::pair<std::string, std::function<void()>>> Options =
{
	{"Play", []() {
		auto scr = UICanvas::CreateNewCanvas<TextScreen>();

		scr->AddText("Find 25 fuel cans to win.");
		scr->AddText("the expansion of the factory, which was built to scale.");
		scr->AddText("The overseer bot has instructed you to collect fuel for");
		scr->TargetScene = "Tutorial";

		Input::CursorVisible = false;
	}},
	{"Full screen", []() {
		Window::SetFullScreen(!Window::GetFullScreen());
	}},
	{"Exit", []() {
		Application::Quit();
	}},
};

MenuUI::MenuUI()
{
	auto ButtonsBackground = (new UIBackground(UIBox::Orientation::Vertical, Vector2(-0.9f, -0.25f), 0, Vector2(0.3f, 0.5f)))
		->SetOpacity(0.75f)
		->SetBorder(UIBox::BorderType::Rounded, 0.5f);

	int Index = 0;
	for (const auto& Option : Options)
	{
		UIButton* NewButton = new UIButton(UIBox::Orientation::Horizontal, 0, 1, this, Index++);

		ButtonsBackground->AddChild(NewButton
			->SetTryFill(true)
			->SetPadding(0.025f)
			->SetPaddingSizeMode(UIBox::SizeMode::AspectRelative)
			->AddChild((new UIText(1, 0, Option.first, GameUI::GetFont()))
				->SetPadding(0.01f)));

		Buttons.push_back(NewButton);
	}

}

void MenuUI::Tick()
{
	if (!Active)
		return;
	for (const auto& i : Input::Gamepads)
	{
		GamepadInput(i.second);
	}


	if (UsedController)
	{
		for (int i = 0; i < Buttons.size(); i++)
		{
			Buttons[i]->SetColor(i == SelectedOption ? 0.5f : 1.0f);
		}
	}
}

Application::Timer InputTimer;
void MenuUI::GamepadInput(const Input::Gamepad& Target)
{
	if (InputTimer.Get() < 0.25f)
	{
		return;
	}
	if (Target.DPadLocation.Y < -0.25f || Target.LeftStickPosition.Y < -0.25f)
	{
		SelectedOption++;
		UsedController = true;
		InputTimer.Reset();
	}
	if (Target.DPadLocation.Y > 0.25f || Target.LeftStickPosition.Y > 0.25f)
	{
		SelectedOption--;
		UsedController = true;
		InputTimer.Reset();
	}

	if (Target.IsButtonDown(Input::GAMEPAD_A))
	{
		UsedController = true;
		OnButtonClicked(SelectedOption);
		InputTimer.Reset();
	}

	SelectedOption = std::clamp(SelectedOption, 0, int(Options.size()));
}

void MenuUI::OnButtonClicked(int Index)
{
	if (!Active)
		return;
	if (Index == 0)
	{
		Active = false;
	}
	Options[Index].second();
}
