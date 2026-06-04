#include "Sound.h"

Sound::Sound(const char* filename_) : audioData(nullptr){
	filename = filename_;
}

Sound::~Sound() { /* free memory */ };

bool Sound::OnCreate(MIX_Mixer* audioMixer) {
	loadSound(filename, audioMixer);
	return true;
}

void Sound::OnDestroy() {
	if (audioData) {
		MIX_DestroyAudio(audioData);
		audioData = nullptr;
	}
}

void Sound::loadSound(const char* filename_, MIX_Mixer* audioMixer) {
	audioData = MIX_LoadAudio(audioMixer, filename_, true);
	if (!audioData) {
		throw std::runtime_error("Fail to load track");
	}
}