#ifndef OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H
#define OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H

/**
 * @file System/sound_manager.h
 * @brief Elementary manager class to manage sounds.
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
 * @brief This class manages rich sounds like a background music, effect sounds,
 * anything related to sound from WAV file, OGG file...
 * SoundManager is not able to copy, move and have property of singleton. So you have to call
 * GetInstance() to use SoundManager.
 *
 * Support audio file type in present :: WAV, OGG.
 * @bug Although sound mute status is on, sound on playing actually does not stop.
 * @todo Loading wave file mechanism is deprecated now, should chagne it to recommended code.
 */
class SoundManager final {
public:
	/**
	 * @brief SoundType is type of each sounds to have been storing.
	 */
	enum class SoundType {
		EFFECT,		/** This is for effect sound, once play but not looped normally. */
		BACKGROUND, /** This is for background sound, looped normally. */
		SURROUND	/** This is for 3D surround ambient sound have distance. */
	};
	/** FileType is used to specify what file type sound file to read has. */
	enum class FileType { WAV, OGG, NONE };

	/**
	 * @brief Thie class stores sound information.
	 */
	class SoundInfo {
	public:
		ALuint source;	/** Sound source stands for origin of sound stream. */
		ALuint buffer;	/** Sound stream actually sound information is stored to. */
		SoundType type;	/** The type of sound. */

		SoundInfo(const ALuint source, const ALuint buffer, SoundType type) :
			source{ source }, buffer{ buffer }, type{ type } {};
	};

public:
	/**
     * @brief Static method gets unique instance of SoundManager class.
	 */
	static SoundManager& GetInstance() {
		static SoundManager instance{};
		return instance;
	}

	/**
	 * @brief Creates sound.
	 * @param[in] tag The tag to name sound object.
	 * @param[in] path The file path to load.
	 * @param[in] sound_type The type of sound, EFFECT, BACKGROUND, SURROUND.
	 * @param[in] file_type The type of file, default value is NONE. Thereby this method retrieve
	 * file's file type automatically.
	 * @return The success flag, return true if success.
	 *
	 * @todo Implement detection file type when file_type is FileType::NONE.
	 */
	bool InsertSound(const std::string&& tag, const std::string&& path, SoundType sound_type,
		FileType file_type = FileType::NONE);

	/**
	 * @brief Destroy sound with tag.
	 * @param[in] tag The tag to be used for searching sound object.
	 * @return The success flag, return true if success.
	 */
	bool DestroySound(const std::string&& tag);

	/**
	 * @brief Play specified sound with tag.
	 *
	 * @param[in] tag The tag to find sound object.
	 */
	 void PlaySound(const std::string&& tag);

	/**
	 * @brief Stop specified sound with tag.
	 * @param[in] tag The tag to stop sound object has that tag name.
	 */
	 void StopSound(const std::string&& tag);

	/**
	 * @brief Stop all of sounds which are playing.
	 */
	 void StopAllSounds();

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
	 * @brief Set mute with true or false.
	 * @param[in] value Mute switch.
	 */
	 inline void SetMute(const bool value);

private:
	ALCdevice* m_device{};		/** Sound device to output. */
	ALCcontext* m_context{};	/** Sound context */

	/** Sound container stores all of sound to be played in application. */
	std::unordered_map<std::string, SoundInfo> m_sounds{};

	bool m_is_muted{ false };	/** Switch if all sounds needs to be sliented. */

private:
	/**
	 * @brief Check there is a error when processing sound stream.
	 * @return If no errors found, return true else false.
	 */
	inline const bool CheckSoundError() const;

	/**
	 * @brief This private method actually processes stopping sound be playing.
	 * This must be called StopSound method and StopAllSounds method.
	 * @param[in] sound The sound container to stop.
	 */
	 void ProcessStopSound(const SoundInfo& sound);

	/**
	 * @brief Initiate "Wav" sound file.
	 * This method must be called in InsertSound() method body.
	 *
	 * @param[in] tag The tag to name sound object.
	 * @param[in] path The file path to load.
	 * @return Success flag.
	 */
	bool InitiateWavSound(const std::string& tag, const std::string& path);

	/**
	 * @brief Initiate "Ogg" sound file.
	 * This method is not implemented yet, always return false.
	 * @param[in] tag The tag to name sound object.
	 * @param[in] path The file path to load.
	 * @return Success flag.
	 */
	bool InitiateOggSound(const std::string& tag, const std::string& path);

private:
	/**
	 * @brief As soon as singleton class has given memory space to be speicified,
	 * SoundManager apply OpenAL for openning device automatically.
	 */
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

inline const bool SoundManager::IsSoundMuted() const { return m_is_muted; }

inline void SoundManager::SetMute(const bool value) { m_is_muted = value; }

inline const bool SoundManager::CheckSoundError() const {
	ALCenum sound_error = alGetError();
	if (sound_error != AL_NO_ERROR) {
		std::cerr << "ERROR::SOUND::AL::SOMETHING::HAPPENDED" << std::endl;
		return false;
	}
	return true;
}

#endif /** OPENGL_TUTORIALS_SYSTEM_SOUND_MANAGER_H */
