#ifndef OPGS16_SYSTEM_SOUND_MANAGER_H
#define OPGS16_SYSTEM_SOUND_MANAGER_H

/**
 * @file System/sound_manager.h
 * @brief Elementary manager class to manage sounds.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @date 2018-02-26
 *
 * @log
 * 2018-02-26 Change sound library from OpenAL & alut to Fmod.
 */

#include <unordered_map>    /*! std::unordered_map */
#include <fmod.hpp>         /*! FMOD */

/**
 * @class SoundManager
 * @brief This class manages rich sounds like a background music, effect sounds,
 * anything related to sound from WAV file, OGG file...
 * SoundManager is not able to copy, move and have property of singleton. So you have to call
 * Instance() to use SoundManager.
 *
 * @bug
 * Although sound mute status is on, sound on playing actually does not stop.
 * Looping is not completely clean. some delay has been created between looped.
 * @todo Implement complete sound features.
 * 1. Stop sound feature and pause also.
 * 2. Multiple stimutaneous effect playing.
 *
 * @date 2018-02-26
 *
 * @log
 * 2018-02-26 Replace OpenAL with FMOD.
 */
class SoundManager final {
public:
	/*! SoundType is type of each sounds to have been storing. */
	enum class SoundType {
		EFFECT,		/** This is for effect sound, once play but not looped normally. */
		BACKGROUND, /** This is for background sound, looped normally. */
		SURROUND	/** This is for 3D surround ambient sound have distance. */
	};

	/*! This class stores sound information. */
    class SoundInfo {
    private:
        FMOD::Sound*    m_sound;    /*! Sound buffer */
		const SoundType m_type;	    /*! The type of sound. */

    public:
        explicit SoundInfo(FMOD::Sound* sound, SoundType type) :
            m_sound{ sound }, m_type{ type } {};

        FMOD::Sound* const Sound() const { return m_sound; }
	};

public:
	/**
     * @brief Static method returns unique instance of SoundManager class.
	 */
	static SoundManager& GetInstance() {
		static SoundManager instance{};
		return instance;
	}

	/**
	 * @brief As soon as singleton class has given memory space to be speicified,
	 * SoundManager apply OpenAL for openning device automatically.
	 */
    bool ProcessInitialSetting();

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
	bool CreateSound(const std::string& tag, const std::string& path, SoundType sound_type);

    /*!
     * @brief Get sound resource and initialize sound.
     * First, Check sound container if any already sound with same tag exists.
     * and nothing found, get a resource from ResourceManager and initialize.
     * or found sound item with same tag, do nothing and just exit method.
     *
     * @param[in] item_tag The tag to name to sound object.
     */
    bool CreateSound(const std::string& item_tag);

	/**
	 * @brief Destroy sound with tag.
	 * @param[in] tag The tag to be used for searching sound object.
	 * @return The success flag, return true if success.
	 */
	bool DestroySound(const std::string& tag);

	/**
	 * @brief Play specified sound with tag.
	 * @param[in] tag The tag to find sound object.
	 */
	void PlaySound(const std::string& tag);

	/**
	 * @brief Stop specified sound with tag.
	 * @param[in] tag The tag to stop sound object has that tag name.
	 */
	void StopSound(const std::string& tag);

	/*! Stop all of sounds which are playing. */
	void StopAllSounds();

    /*! Release all sounds, but leaves system on active. */
    void Clear();

	/**
	 * @brief Check if any sound stream with 'tag' name is exist.
	 * @return If duplicated name is exist already, return true, elsewhere false.
	 */
	inline const bool DoesSoundExist(const std::string& tag) const;

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
    FMOD::System*   m_system;
    FMOD_RESULT     m_result;
    unsigned        m_version;
    int             m_sound_driver_count;
    FMOD::Channel*  m_sound_channel{ nullptr };

	/** Sound container stores all of sound to be played in application. */
	std::unordered_map<std::string, SoundInfo> m_sounds{};

	bool m_is_muted{ false };	/** Switch if all sounds needs to be sliented. */
    mutable bool m_not_initiated{ true };

private:
	/**
	 * @brief This private method actually processes stopping sound be playing.
	 * This must be called StopSound method and StopAllSounds method.
	 * @param[in] sound The sound container to stop.
	 */
	 void ProcessStopSound(const SoundInfo& sound);

private:
	/**
	 * @brief Stop all playing sounds, and release resources.
	 * This destructor called when Application ends.
	 */
    SoundManager() = default;
	~SoundManager();

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
};

inline const bool SoundManager::DoesSoundExist(const std::string& tag) const {
	return m_sounds.find(tag) != m_sounds.end();
}

inline const bool SoundManager::IsSoundMuted() const {
    return m_is_muted;
}

inline void SoundManager::SetMute(const bool value) {
    m_is_muted = value;
}

#endif /** OPGS16_SYSTEM_SOUND_MANAGER_H */
