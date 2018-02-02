#include "sound_manager.h"
#include <iostream>

SoundManager::SoundManager() {
	/** Find device automatically */
	m_device = alcOpenDevice(nullptr);
	if (!m_device) {
		std::cerr << "ERROR::NOT::FOUND::SOUND::DEVICE" << std::endl;
	}
	/** In order to render audio scene, create and initialize a context */
	m_context = alcCreateContext(m_device, nullptr);
	if (!alcMakeContextCurrent(m_context) && !CheckSoundError()) {
		std::cerr << "ERROR::FAILED::TO::CREATE::SOUND::CONTEXT" << std::endl;
	}
}

SoundManager::~SoundManager() {
	StopAllSounds();

	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
		alDeleteSources(1, &sound.source);
		alDeleteBuffers(1, &sound.buffer);
	}

	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

bool SoundManager::InsertSound(const std::string&& tag,
	const std::string&& path, SoundType sound_type, FileType file_type) {
	/** Body */
	if (IsSoundExist(tag)) return false;

	FileType actual_file_type{ FileType::NONE };
	if (file_type == FileType::NONE) {
		// Figure out what type of file is.
	}
	else actual_file_type = file_type;

	auto success_flag = false;
	switch (actual_file_type) {
	case FileType::WAV: success_flag = InitiateWavSound(tag, path); break;
	case FileType::OGG: success_flag = InitiateOggSound(tag, path); break;
	default:
		std::cerr << "ERROR::SOUND::TYPE::NOT::CORRECT" << std::endl;
		break;
	}

	return success_flag;
}

bool SoundManager::DestroySound(const std::string&& tag) {
	if (IsSoundExist(tag)) {
		StopSound(std::string(tag));

		auto& sound = m_sounds.at(tag);
		alDeleteSources(1, &sound.source);
		alDeleteBuffers(1, &sound.buffer);
		m_sounds.erase(tag);

		return true;
	}
	else return false;
}

void SoundManager::PlaySound(const std::string&& tag) {
	if (IsSoundExist(tag)) {
		auto& sound = m_sounds.at(tag);
		/** Bind source to buffer, in order to actually output sound. */
		alSourcei(sound.source, AL_BUFFER, sound.buffer);
		alSourcePlay(sound.source);
		CheckSoundError();
	}
}

void SoundManager::StopSound(const std::string&& tag) {
	if (IsSoundExist(tag)) {
		auto& sound = m_sounds.at(tag);
		ProcessStopSound(sound);
	}
}

void SoundManager::StopAllSounds() {
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
		ProcessStopSound(sound);
	}
}

void SoundManager::ProcessStopSound(const SoundInfo& sound) {
	ALint state;
	alGetSourcei(sound.source, AL_SOURCE_STATE, &state);
	CheckSoundError();

	if (state == AL_PLAYING) {
		alSourceStop(sound.source);
		CheckSoundError();
	}
}

bool SoundManager::InitiateWavSound(const std::string& tag, const std::string& path) {
	/** Must create the source which is actually the origin of sound. */
	ALuint source;
	alGenSources(1, &source);			if (!CheckSoundError()) return false;
	alSourcef(source, AL_PITCH, 1);		if (!CheckSoundError()) return false;

	/** Must create the buffer which stores stream of sound from source */
	ALuint buffer;
	alGenBuffers(1, &buffer);			if (!CheckSoundError()) return false;

	/** And load sound stream to buffer */
	ALsizei size, freq; ALenum format; ALvoid *data;
	ALboolean loop = AL_FALSE;

	alutLoadWAVFile(static_cast<ALbyte*>(const_cast<char*>(path.c_str())),
		&format, &data, &size, &freq, &loop);
	if (!CheckSoundError()) return false;
	alBufferData(buffer, format, data, size, freq);

	/** Insert container */
	m_sounds.insert(std::make_pair(tag, SoundInfo{ source, buffer, SoundType::EFFECT }));
	return true;
}

bool SoundManager::InitiateOggSound(const std::string & tag, const std::string & path) {
	std::cerr << "ERROR::SOUND::NOT::IMPLEMENTED::YET" << std::endl;
	return false;
}

