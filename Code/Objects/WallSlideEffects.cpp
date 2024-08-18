#include "WallSlideEffects.h"

void WallSlideEffects::Begin()
{
	Attach(SlideParticle);
	SlideParticle->LoadParticle("WallSlide");
	SlideParticle->SetActive(false);

	Attach(JumpParticle);
	JumpParticle->LoadParticle("WallJump");
	JumpParticle->SetActive(false);
}

void WallSlideEffects::Update()
{
}

void WallSlideEffects::StartSlide()
{
	if (!SlideParticle->GetActive())
	{
		JumpParticle->SetActive(false);
		JumpParticle->Reset();
		SlideParticle->Reset();
	}
	SlideParticle->SetActive(true);
}

void WallSlideEffects::Stop()
{
	SlideParticle->SetActive(false);
}

void WallSlideEffects::WallJump()
{
	JumpParticle->SetActive(true);
	JumpParticle->Reset();
}
