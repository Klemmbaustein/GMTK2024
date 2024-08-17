#pragma once
#include <GENERATED/WallSlideEffects.h>
#include <Objects/SceneObject.h>
#include <Objects/Components/ParticleComponent.h>

class WallSlideEffects : public SceneObject
{
public:
	WALLSLIDEEFFECTS_GENERATED("Game/Misc");

	void Begin() override;
	void Update() override;

	void StartSlide();
	void Stop();
	void WallJump();
private:
	ParticleComponent* SlideParticle = new ParticleComponent();
	ParticleComponent* JumpParticle = new ParticleComponent();
	bool IsActive = false;
};