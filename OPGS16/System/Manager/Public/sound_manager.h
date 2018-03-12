#ifndef OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/sound_manager.h
 * @brief Elementary manager class to manage sounds.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-26 Change sound library from OpenAL & alut to Fmod.
 * 2018-03-04 Refactoring.
 */

#include <unordered_map>    /*! std::unordered_map */
#include <fmod.hpp>         /*! FMOD */

#include "../Internal/sound_internal.h"

namespace opgs16 {
namespace manager {

/*!
 * @class MSoundManager
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
 * @log
 * 2018-02-26 Replace OpenAL with FMOD.
 * 2018-03-04 Refactoring, Move class in opgs16::manager.
 */
class MSoundManager final {
public:
	/*! Returns unique instance of SoundManager class. */
	static MSoundManager& Instance() {
		static MSoundManager instance{};
		return instance;
	}

	/**
	 * @brief As soon as singleton class has given memory space to be speicified,
	 * SoundManager apply OpenAL for openning device automatically.
	 */
    bool ProcessInitialSetting();

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
	inline bool DoesSoundExist(const std::string& tag) const;

	/**
	 * @brief Return true or false whenever sound is muted or not.
	 * @return If sound is muted, return true, else return false.
	 */
	inline bool IsSoundMuted() const;

	/**
	 * @brief Set mute with true or false.
	 * @param[in] value Mute switch.
	 */
	inline void SetMute(const bool value);

private:
    FMOD::System*   m_system{ nullptr };
    FMOD_RESULT     m_result{ FMOD_OK };
    unsigned        m_version{ 0 };
    int             m_sound_driver_count{ 0 };
    FMOD::Channel*  m_sound_channel{ nullptr };

	/** Sound container stores all of sound to be played in application. */
	std::unordered_map<std::string, _internal::SSoundInfo> m_sounds{};

	bool m_is_muted{ false };	/** Switch if all sounds needs to be sliented. */
    mutable bool m_not_initiated{ true };

private:
	/**
	 * @brief This private method actually processes stopping sound be playing.
	 * This must be called StopSound method and StopAllSounds method.
	 * @param[in] sound The sound container to stop.
	 */
	void ProcessStopSound(const _internal::SSoundInfo& sound);

	/**
	 * @brief Stop all playing sounds, and release resources.
	 * This destructor called when Application ends.
	 */
    MSoundManager() = default;

public:
	~MSoundManager();
	MSoundManager(const MSoundManager&) = delete;
	MSoundManager& operator=(const MSoundManager&) = delete;
};

inline bool MSoundManager::DoesSoundExist(const std::string& tag) const {
	return m_sounds.find(tag) != m_sounds.end();
}

inline bool MSoundManager::IsSoundMuted() const {
    return m_is_muted;
}

inline void MSoundManager::SetMute(const bool value) {
    m_is_muted = value;
}

} /*! opgs16::manager */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H */
