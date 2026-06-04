#include "SoundManager.h"

SoundManager::SoundManager() : audioMixer(nullptr), BGMTrack(nullptr) {
	for (size_t i = 0; i < (size_t)SFX_TRACKS::MAX_TRACKS; i++) {
		SFXTracks.push_back(nullptr);
	}
}

SoundManager::~SoundManager() { /* Free Memory */ }

bool SoundManager::OnCreate() {
	audioMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &defaultSpec);

	if (!audioMixer) {
		throw std::runtime_error("Fail to create mixer");
	}

	// create for SFX
	for (MIX_Track*& SFXtrack : SFXTracks) {
		SFXtrack = MIX_CreateTrack(audioMixer);
		if (!SFXtrack) {
			throw std::runtime_error("Fail to load track");
		}
	}

	// create for BGM
	BGMTrack = MIX_CreateTrack(audioMixer);
	if (!BGMTrack) {
		throw std::runtime_error("Fail to load track");
	}

	return true;
}

void SoundManager::OnDestroy() {
	for (MIX_Track*& SFXtrack : SFXTracks) {
		if (SFXtrack) {
			MIX_DestroyTrack(SFXtrack);
			SFXtrack = nullptr;
		}
	}

	if (BGMTrack) {
		MIX_DestroyTrack(BGMTrack);
		BGMTrack = nullptr;
	}

	if (audioMixer) {
		MIX_DestroyMixer(audioMixer);
		audioMixer = nullptr;
	}
}

// background music specific
void SoundManager::PlayBGM(const Sound* BGM) const{
	MIX_StopTrack(BGMTrack, 0);
	MIX_SetTrackAudio(BGMTrack, BGM->getAudioData());
	MIX_PlayTrack(BGMTrack, -1);
}

void SoundManager::PlaySFX(const Sound* SFX) const {
	MIX_PlayAudio(audioMixer, SFX->getAudioData());
}

void SoundManager::adjustMasterVolume(const float value) {
	MIX_SetMixerGain(audioMixer, value);
}

void SoundManager::adjustBackgroundMusicVolume(const float value) {
	MIX_SetTrackGain(BGMTrack, value);
}

void SoundManager::adjustSFXVolume(const float value) {
	for (MIX_Track* SFXtrack : SFXTracks) {
		MIX_SetTrackGain(SFXtrack, value);
	}
}