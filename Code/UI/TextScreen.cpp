#include "TextScreen.h"
#include <UI/UIText.h>
#include "GameUI.h"
#include <Engine/Subsystem/Scene.h>

TextScreen::TextScreen()
{
	bg->SetVerticalAlign(UIBox::Align::Centered);
	bg->SetHorizontalAlign(UIBox::Align::Centered);
}

void TextScreen::Tick()
{
	if (t.Get() > 5)
	{
		Scene::LoadNewScene(TargetScene);
	}
}

void TextScreen::AddText(std::string txt)
{
	bg->AddChild(new UIText(1, 1, txt, GameUI::GetFont()));
}
