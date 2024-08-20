#include "GameUI.h"
#include <Engine/Log.h>
#include <Objects/Player.h>
#include <Engine/Utility/StringUtility.h>

TextRenderer* GameUI::UIFont = nullptr;

TextRenderer* GameUI::GetFont()
{
	if (!UIFont)
	{
		UIFont = new TextRenderer("Roboto.ttf");
	}

	return UIFont;
}

GameUI::GameUI()
{
	// Ensure the font exists at this point.
	TextRenderer* Font = GetFont();
	StatusBackground
		->SetOpacity(0.75f)
		->SetBorder(UIBox::BorderType::Rounded, 0.5f);

	StatusBackground
		->SetVerticalAlign(UIBox::Align::Centered)
		->AddChild((new UIBackground(UIBox::Orientation::Horizontal, 0, 1, 0.075f))
			->SetUseTexture(true, "FuelCanRender")
			->SetSizeMode(UIBox::SizeMode::AspectRelative)
			->SetPadding(0, 0, 0.02f, 0.02f))
		->AddChild(ScoreText);
}

void GameUI::Tick()
{
	ScoreText->SetText(StrUtil::Format("Score: %i", (int)Player::Score));

	if (TransitionProgress < 2)
	{
		TransitionBackground->IsVisible = true;

		float Position;
		if (TransitionProgress < 1)
		{
			Position = 2 - pow(std::min(TransitionProgress * 1.25f, 1.0f), 2.0) * 2;
		}
		else
		{
			Position = pow(std::min((TransitionProgress - 1.0f) * 1.25f, 1.0f), 2.0) * 2;
		}

		TransitionBackground->SetPosition(Vector2(-1 + Position, -1));
		TransitionProgress += Stats::DeltaTime;

		if (TransitionProgress >= 1.0f && TransitionProgress - Stats::DeltaTime < 1.0f)
		{
			if (TransitionEndFunction)
				TransitionEndFunction();
			TransitionEndFunction = nullptr;
		}
	}
	else
	{
		TransitionBackground->IsVisible = false;
	}
}

void GameUI::StartTransition(std::function<void()> EndFunction)
{
	if (InTransition())
		return;
	TransitionBackground->SetRenderOrderIndex(SIZE_MAX);
	TransitionProgress = 0;
	TransitionEndFunction = EndFunction;
}

bool GameUI::InTransition() const
{
	return TransitionProgress < 2;
}
