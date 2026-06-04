#pragma once
#include <SDL3/SDL_audio.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL.h>
#include <Vector.h>

class Sound {
private:
	MIX_Audio* audioData;
	const char* filename;

	// load sound
	void loadSound(const char* filename_, MIX_Mixer* audioMixer);

public:
	Sound(const char* filename_);
	~Sound();

	bool OnCreate(MIX_Mixer* audioMixer);
	void OnDestroy();

	MIX_Audio* getAudioData() const { return audioData; }
};