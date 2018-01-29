#ifndef OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H
#define OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H

/**
 * @file System/sound_manager.h
 * @brief Elementary manager class to manage post-processing shaders.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <al.h>
#include <alc.h>
#include <AL\alut.h>

/**
 * @class SoundManager
 * @brief
 */
class SoundManager {
public:
	enum class SoundType { EFFECT, BACKGROUND, SURROUND };
	enum class FileType { WAV, OGG, NONE };

	/**
	 * @brief Store sound information.
	 */
	class SoundInfo {
	public:
		ALuint source;
		ALuint buffer;
		SoundType type;

		SoundInfo(const ALuint source, const ALuint buffer, SoundType type) :
			source{ source }, buffer{ buffer }, type{ type } {};
	};

public:
	/**
	 * @brief
	 */
	static SoundManager& GetInstance() {
		static SoundManager instance{};
		return instance;
	}

	/**
	 * @brief Creates sound.
	 * @param[in] tag
	 * @param[in] path
	 * @param[in] sound_type
	 * @return The success flag, return true if success.
	 */
	bool InsertSound(const std::string&& tag, const std::string&& path, SoundType sound_type,
		FileType file_type = FileType::NONE);

	/**
	 * @brief Destroy sound with tag.
	 * @param[in] tag
	 * @return The success flag, return true if success.
	 */
	bool DestroySound(const std::string&& tag);

	/**
	 * @brief Play specified sound with tag.
	 *
	 *
	 * @param[in] tag
	 */
	[[noreturn]] void PlaySound(const std::string&& tag);

	/**
	 * @brief Stop specified sound with tag.
	 * @param[in] tag
	 */
	[[noreturn]] void StopSound(const std::string&& tag);

	/**
	 * @brief Stop all of sounds which are playing.
	 */
	[[noreturn]] void StopAllSounds();

	/**
	 * @brief Check if any sound stream with 'tag' name is exist.
	 * @return If duplicated name is exist already, return true, elsewhere false.
	 */
	inline const bool IsSoundExist(const std::string tag) const;

	/**
	 * @brief Return true or false whenever sound is muted or not.
	 * @return If sound is muted, return true, else return false.
	 */
	inline const bool IsSoundMuted() const;

	/**
	 * @brief Set
	 * @param[in] value
	 */
	[[noreturn]] inline void SetMute(const bool value);

private:
	ALCdevice* m_device{};		/** */
	ALCcontext* m_context{};	/** */

	std::unordered_map<std::string, SoundInfo> m_sounds{};

	bool m_is_muted{ false };

private:
	/**
	 * @brief Check there is a error when processing sound stream.
	 * @return If no errors found, return true else false.
	 */
	inline const bool CheckSoundError() const;

	/**
	 * @brief
	 */
	void ProcessStopSound(const SoundInfo& manager);

	/**
	 * @brief Initiate "Wav" sound file.
	 * This method must be called in InsertSound() method body.
	 *
	 * @param[in] tag
	 * @param[in] path
	 * @return Success flag.
	 */
	bool InitiateWavSound(const std::string& tag, const std::string& path);

	/**
	 * @brief Initiate "Ogg" sound file.
	 * This method is not implemented yet, always return false.
	 */
	bool InitiateOggSound(const std::string& tag, const std::string& path);

private:
	SoundManager();

	/**
	 * @brief Stop all playing sounds, and release resources.
	 * This destructor called when Application ends.
	 */
	~SoundManager();

	/** Not used */
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(const SoundManager&&) = delete;
	SoundManager& operator=(const SoundManager&&) = delete;
};

inline const bool SoundManager::IsSoundExist(const std::string tag) const {
	return m_sounds.find(tag) != m_sounds.end();
}

inline const bool SoundManager::IsSoundMuted() const {
	return m_is_muted;
}

inline void SoundManager::SetMute(const bool value) {
	m_is_muted = value;
}

inline const bool SoundManager::CheckSoundError() const {
	ALCenum sound_error = alGetError();
	if (sound_error != AL_NO_ERROR) {
		std::cerr << "ERROR::SOUND::AL::SOMETHING::HAPPENDED" << std::endl;
		return false;
	}
	return true;
}

#endif /** OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H */
