#include "FuelCan.h"
#include <Objects/Player.h>
#include <Objects/Components/MeshComponent.h>
#include <Objects/ParticleObject.h>
#include <Engine/Subsystem/Scene.h>
#include <UI/TextScreen.h>
#include <Engine/Log.h>
#include <Sounds.h>

static std::set<std::string> PickedUp = {};

static std::string GetCanID(FuelCan* c)
{
	return Scene::CurrentScene + c->GetTransform().Position.ToString();
}

void FuelCan::Begin()
{
	MeshComponent* Mesh = new MeshComponent();
	Attach(Mesh);
	Mesh->Load("FuelCan");
	Mesh->RelativeTransform.Position.Y = 4;
	Mesh->CastStaticShadow = false;

	if (PickedUp.contains(GetCanID(this)))
	{
		Objects::DestroyObject(this);
	}
}

void FuelCan::Update()
{
#if EDITOR
	return;
#endif
	GetTransform().Rotation = Vector3(sin(Stats::Time) * 45, Vector3(Stats::Time).RadiansToDegrees().X, 0);

	if (Vector3::Distance(Player::Current->GetTransform().Position, GetTransform().Position) < std::max(7 * Player::Current->GetScaleValue(), 7.0f))
	{
		Objects::DestroyObject(this);
		Transform SpawnTransform = GetTransform();
		SpawnTransform.Rotation = 0;
		Objects::SpawnObject<ParticleObject>(SpawnTransform)->LoadParticle("FuelCanPickup");
		Player::Score++;
		PickedUp.insert(GetCanID(this));
		Sound::PlaySound2D(Sounds::FuelCan, 1, 0.5f);

		if (Player::Score >= 25)
		{
			auto scr = UICanvas::CreateNewCanvas<TextScreen>();
			scr->AddText("Thanks for playing!");
			scr->AddText("You have collected enough fuel for the robot factory to expand more.");
			scr->TargetScene = "Menu";
			Player::Current->Movement->Active = false;
			Player::Current->HasControl = false;
		}
	}
}
