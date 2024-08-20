#pragma once
#include <UI/UICanvas.h>
#include <UI/UIText.h>
#include <UI/UIBackground.h>
#include <Engine/Application.h>

class TextScreen : public UICanvas
{
public:

	TextScreen();

	void Tick() override;

	void AddText(std::string txt);

	UIBackground* bg = new UIBackground(UIBox::Orientation::Vertical, -1, 0, 2);

	std::string TargetScene;

	Application::Timer t;
};