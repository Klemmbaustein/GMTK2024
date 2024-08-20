#pragma once
#include <UI/UICanvas.h>
#include <UI/UIBackground.h>
#include <functional>
#include <UI/UIText.h>

class GameUI : public UICanvas
{
public:

	static TextRenderer* GetFont();

	GameUI();
	void Tick() override;

	void StartTransition(std::function<void()> EndFunction);
	bool InTransition() const;

private:
	UIText* ScoreText = new UIText(1, 1, "", GetFont());

	UIBackground* TransitionBackground = new UIBackground(UIBox::Orientation::Horizontal, -1, 0, 2);
	UIBackground* StatusBackground = new UIBackground(UIBox::Orientation::Horizontal, Vector2(0.5f, 0.8f), Vector3(0), Vector2(0.4f, 0.1f));

	static TextRenderer* UIFont;
	float TransitionProgress = 3;
	std::function<void()> TransitionEndFunction = nullptr;
};