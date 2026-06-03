#pragma once
#include "Sound.h"
#include <vector>
#include <string>

class SoundManager {
public:
	SoundManager();
	~SoundManager();

	bool OnCreate();
	void OnDestroy();

	void playSoundAt(const Sound* sound, const int pipe);
	void playSoundAt(const Sound* sound); // automaticlly select a pipe that no sound is played

private:
	std::vector<SDL_AudioStream*> audioStreamList;
	SDL_AudioDeviceID mainDevice;

	enum class SOUND_PIPE {
		PIPE1 = 0,
		PIPE2,
		PIPE3,
		PIPE4,
		PIPE5,
		PIPE6,
		PIPE7,
		PIPE8,
		PIPE9,
		PIPE10,
		PIPE11,
		PIPE12,
		MAX_NUMBER_PIPES
	};

	SDL_AudioSpec defaultSpec{ // default WAV format spec
		SDL_AUDIO_S16, // format
		2, // channel
		48000 // frequency
	};
};