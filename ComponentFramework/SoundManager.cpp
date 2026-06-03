#include "SoundManager.h"

SoundManager::SoundManager() {
	mainDevice = SDL_OpenAudioDevice(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&defaultSpec
	);
}

SoundManager::~SoundManager() { /* Free Memory */ }

bool SoundManager::OnCreate() {
	for (size_t i = 0; i < (size_t)SOUND_PIPE::MAX_NUMBER_PIPES; i++) {
		audioStreamList.push_back(SDL_CreateAudioStream(&defaultSpec, &defaultSpec));
		SDL_BindAudioStream(mainDevice, audioStreamList[i]);
	}
	return true;
}

void SoundManager::OnDestroy() {
	for (SDL_AudioStream* audioPipe: audioStreamList) {
		SDL_DestroyAudioStream(audioPipe);
	}
}

void SoundManager::playSoundAt(const Sound* sound, const int pipe) {
	SDL_ClearAudioStream(audioStreamList[pipe]);
	sound->Play(audioStreamList[pipe]);
}