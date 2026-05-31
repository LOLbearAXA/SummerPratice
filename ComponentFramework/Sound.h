#pragma once
#include <SDL3/SDL_audio.h>
#include <SDL.h>
#include <Vector.h>

class Sound {
private:
	SDL_AudioSpec spec;
	Uint8* waveBuffer;
	Uint32 waveLength;

	const char* filename;

	// load sound
	void loadSound(const char* filename_);

public:
	Sound(const char* filename_);
	~Sound();

	bool OnCreate();
	void OnDestroy();

	void Play(SDL_AudioStream* audioPlayer);
};