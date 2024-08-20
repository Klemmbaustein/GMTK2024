#include "Sounds.h"

namespace Sounds
{
	bool Loaded = false;
	Sound::SoundBuffer* Jump;
	Sound::SoundBuffer* WallJump;
	Sound::SoundBuffer* DoubleJump;
	Sound::SoundBuffer* WaterSplash;
	Sound::SoundBuffer* Walk;
	Sound::SoundBuffer* FuelCan;
	Sound::SoundBuffer* Impact;
	Sound::SoundBuffer* Jetpack;
	Sound::SoundBuffer* Checkpoint;
}

void Sounds::Load()
{
	if (Loaded)
	{
		return;
	}
	Loaded = true;
	Jump = new Sound::SoundBuffer("Jump");
	WallJump = new Sound::SoundBuffer("WallJump");
	DoubleJump = new Sound::SoundBuffer("DoubleJump");
	WaterSplash = new Sound::SoundBuffer("WaterSplash");
	Walk = new Sound::SoundBuffer("Walk");
	FuelCan = new Sound::SoundBuffer("FuelCan");
	Impact = new Sound::SoundBuffer("Impact");
	Jetpack = new Sound::SoundBuffer("Jetpack");
	Checkpoint = new Sound::SoundBuffer("Checkpoint");
}
