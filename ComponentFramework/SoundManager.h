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

private:
	std::vector<SDL_AudioStream*> audioStreamList;

	enum class SOUND_PIPE {
		PIPE1 = 0,
		PIPE2,
		MAX_NUMBER_PIPES
	};


};