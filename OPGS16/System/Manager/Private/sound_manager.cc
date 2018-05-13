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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Manager/Private/sound_manager.cc
 * @brief Sound manager implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 * 2018-05-13 Changed std::cerr to PUSH_LOG_ERRO logger routine.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/sound_manager.h"        /*! Header file */

#include <string>                           /*! std::string */

#include "../Public/resource_manager.h"     /*! MResourceManager */
#include "../Public/resource_type.h"
#include "../../../Headers/import_logger.h" /*! import logger in debug mode. */

namespace opgs16 {
namespace manager {

namespace {

constexpr bool FAILED   = false;
constexpr bool SUCCESS  = true;

} /*! unnamed namespace */

using _internal::ESoundType;
using _internal::SSoundInfo;

bool MSoundManager::ProcessInitialSetting() {
    if (FMOD::System_Create(&m_system) != FMOD_OK) {
        PUSH_LOG_ERRO("ERROR::DID::NOT::CREATE::SOUND::SYSTEM\n");
        goto label_failed;
    }

    if (m_system->getVersion(&m_version); m_version < FMOD_VERSION) {
        PUSH_LOG_ERRO("ERROR::DOES::NOT::MATCH::FMOD::LIBRARY::VERSION\n");
        goto label_failed;
    }

    if (m_system->getNumDrivers(&m_sound_driver_count); m_sound_driver_count <= 0) {
        PUSH_LOG_ERRO("ERROR::NOT::FOUND::SOUND::DEVICE\n");
        goto label_failed;
    }

    if (m_system->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) {
        PUSH_LOG_ERRO("ERROR::COULD::NOT::CREATE::SOUND::CHANNEL\n");
        goto label_failed;
    }

    goto label_success;

label_failed:
    m_is_muted = true;
    return FAILED;

label_success:
    return SUCCESS;
}

bool MSoundManager::CreateSound(const std::string& item_tag) {
    if (DoesSoundExist(item_tag)) {
        return true;
    }
    else {
        auto& sound_item = MResourceManager::Instance().GetSound(item_tag);

        FMOD::Sound* sound = nullptr;
        auto result = m_system->createSound(sound_item.Path().c_str(), 
                                            FMOD_DEFAULT, 0, &sound);
        if (result != FMOD_OK) {
            char log[255] = {"ERROR::CANNOT::CREATE::SOUND::"};
            std::strcat(log, sound_item.Path().c_str());
            std::strcat(log, "\n");

            PUSH_LOG_ERRO(log);
            return FAILED;
        }

        /*! @todo:Temporary */
        ESoundType sound_type{ ESoundType::BACKGROUND };

        switch (sound_type) {
        case ESoundType::EFFECT:
            sound->setMode(FMOD_LOOP_OFF);
            break;
        case ESoundType::BACKGROUND:
        case ESoundType::SURROUND:
            sound->setMode(FMOD_LOOP_NORMAL);
            sound->setLoopCount(-1);
            break;
        }

        /*! Insert created sound to sound container */
        m_sounds.emplace(std::make_pair(item_tag, SSoundInfo{ sound, sound_type }));
        return SUCCESS;
    }
}

bool MSoundManager::DestroySound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
		StopSound(tag);

        //m_sounds.at(tag).m_sound->release();
        m_sounds.erase(tag);
		return SUCCESS;
	}
	else
        return FAILED;
}

void MSoundManager::PlaySound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
	    auto result = m_system->playSound(m_sounds.at(tag).Sound(), 0, false, &m_sound_channel);
        if (result != FMOD_OK) {
            char log[255] = {"ERROR::CANNOT::PLAY::SOUND::"};
            std::strcat(log, tag.c_str());
            std::strcat(log, "\n");

            PUSH_LOG_ERRO(log);
        }
	}
}

void MSoundManager::StopSound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
		auto& sound = m_sounds.at(tag);
		ProcessStopSound(sound);
	}
}

void MSoundManager::StopAllSounds() {
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
		ProcessStopSound(sound);
	}
}

void MSoundManager::Clear() {
    StopAllSounds();

    /*! Release all sounds */
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
        if (sound.Sound()->release() != FMOD_OK) {
            PUSH_LOG_ERRO("ERROR::SOUND::RELEASE::FAILED\n");
        }
	}
    m_sounds.clear();
}

void MSoundManager::ProcessStopSound(const SSoundInfo& sound) {
    //m_system->getChannel()
}

MSoundManager::~MSoundManager() {
	StopAllSounds();

    /*! Release all sounds */
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
        if (sound.Sound()->release() != FMOD_OK) {
            /*! Write to logger if debug mode. if not and after this, mute app. */
            PUSH_LOG_ERRO("ERROR::SOUND::RELEASE::FAILED\n");
        }
	}
    m_sounds.clear();

    /*! Close and release sound system */
    m_system->close();
    m_system->release();
}

} /*! opgs16::manager */
} /*! opgs16 */

