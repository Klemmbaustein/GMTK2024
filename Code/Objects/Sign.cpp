#include "Sign.h"
#include <Objects/Player.h>
#include <UI/UIBackground.h>
#include <Objects/Components/MeshComponent.h>

void Sign::Begin()
{
	MeshComponent* Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("WoodenSign");

	AddEditorProperty(Property("Text", NativeType::String, &SignString));

#if EDITOR
	return;
#endif

	SignUIBox = (new UIBackground(UIBox::Orientation::Horizontal, 0, 0, 0))
		->SetOpacity(0.75f);
	SignText = new UIText(1, 1, "", GameUI::GetFont());
	SignUIBox->AddChild(SignText
		->SetWrapEnabled(true, 0.75f, UIBox::SizeMode::AspectRelative)
		->SetPadding(0.01f));
	SignUIBox
		->SetBorder(UIBox::BorderType::Rounded, 0.5f);
}

void Sign::Update()
{
#if EDITOR
	return;
#endif

	Vector3 ScreenPosition = Player::Current->PlayerCamera->WorldToScreenPos(GetTransform().Position);

	if (Vector3::Distance(Player::Current->PlayerCamera->GetWorldTransform().Position, GetTransform().Position) < 50
		&& ScreenPosition.Z > 0)
	{
		SignUIBox->SetPosition(Vector2(ScreenPosition.X, ScreenPosition.Y) - SignUIBox->GetUsedSize() / 2);
		SignUIBox->IsVisible = true;
	}
	else
	{
		SignUIBox->IsVisible = false;
	}
}

void Sign::Destroy()
{
}

void Sign::OnPropertySet()
{
#if !EDITOR
	SignText->SetText(SignString);
#endif
}
