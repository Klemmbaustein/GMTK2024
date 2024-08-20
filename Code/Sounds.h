#pragma once
#include <Engine/Subsystem/Sound.h>

namespace Sounds
{
	extern Sound::SoundBuffer* Jump;
	extern Sound::SoundBuffer* WallJump;
	extern Sound::SoundBuffer* DoubleJump;
	extern Sound::SoundBuffer* WaterSplash;
	extern Sound::SoundBuffer* Walk;
	extern Sound::SoundBuffer* FuelCan;
	extern Sound::SoundBuffer* Impact;
	extern Sound::SoundBuffer* Jetpack;
	extern Sound::SoundBuffer* Checkpoint;

	void Load();
}