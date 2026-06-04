#pragma once
#include "Sound.h"
#include <vector>
#include <string>

class SoundManager {
friend Sound;

public:
	SoundManager();
	~SoundManager();

	bool OnCreate();
	void OnDestroy();

	MIX_Mixer* getMixer() const { return audioMixer; }

	// volume adjustment
	// 0.0f - 1.0f
	// 0.0f = 0%
	// 1.0f = 100%
	// 0.5f = 50%
	void adjustBackgroundMusicVolume(const float value);
	void adjustMasterVolume(const float value);
	void adjustSFXVolume(const float value);

	// use on scene update
	void PlayBGM(const Sound* BGM) const;
	void PlaySFX(const Sound* SFX) const; 

private:
	MIX_Mixer* audioMixer;
	MIX_Track* BGMTrack;
	std::vector<MIX_Track*> SFXTracks;

	enum class SFX_TRACKS {
		TRACK1 = 0,
		TRACK2,
		TRACK3,
		TRACK4,
		TRACK5,
		TRACK6,
		TRACK7,
		TRACK8,
		TRACK9,
		TRACK10,
		TRACK11,
		TRACK12,
		MAX_TRACKS
	};

	SDL_AudioSpec defaultSpec{ // default WAV format spec
		SDL_AUDIO_F32, // format
		2, // channel
		48000 // frequency
	};
};