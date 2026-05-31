#include "Sound.h"

Sound::Sound(const char* filename_) : waveBuffer{nullptr}, waveLength{0} {
	filename = filename_;
}

bool Sound::OnCreate() {
	loadSound(filename);
	return true;
}

void Sound::OnDestroy() {
	SDL_free(waveBuffer);
}

void Sound::loadSound(const char* filename_) {

	if (!SDL_LoadWAV(filename_, &spec, &waveBuffer, &waveLength)) {
		throw std::runtime_error("Fail to load sound");
	}
}