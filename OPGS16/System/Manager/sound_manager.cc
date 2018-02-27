#include "sound_manager.h"      /*! Header file */
#include <iostream>             /*! std::cerr */
#include <string>               /*! std::string */
#include "resource_manager.h"   /*! ResourceManager */

constexpr bool FAILED   = false;
constexpr bool SUCCESS  = true;

SoundManager::~SoundManager() {
	StopAllSounds();

    /*! Release all sounds */
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
        if (sound.Sound()->release() != FMOD_OK) {
            /*! Write to logger if debug mode. if not and after this, mute app. */
            std::cerr << "ERROR::SOUND::RELEASE::FAILED\n";
        }
	}
    m_sounds.clear();

    /*! Close and release sound system */
    m_system->close();
    m_system->release();
}

bool SoundManager::ProcessInitialSetting() {
    if (FMOD::System_Create(&m_system) != FMOD_OK) {
        /*! Write to logger if debug mode. in release mode, mute application. */
        std::cerr << "ERROR::DID::NOT::CREATE::SOUND::SYSTEM\n";
        m_is_muted = true;
        return FAILED;
    }

    m_system->getVersion(&m_version);
    if (m_version < FMOD_VERSION) {
        /*! Write to logger if debug mode. if not and after this, mute app. */
        std::cerr << "ERROR::DOES::NOT::MATCH::FMOD::LIBRARY::VERSION\n";
        m_is_muted = true;
        return FAILED;
    }

    m_system->getNumDrivers(&m_sound_driver_count);
    if (m_sound_driver_count <= 0) {
        /*! Write to logger if debug mode. if not and after this, mute app. */
        std::cerr << "ERROR::NOT::FOUND::SOUND::DEVICE\n";
        m_is_muted = true;
        return FAILED;
    }

    if (m_system->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) {
        /*! Write to logger if debug mode. if not and after this, mute app. */
        std::cerr << "ERROR::COULD::NOT::CREATE::SOUND::CHANNEL\n";
        m_is_muted = true;
        return FAILED;
    }

    return SUCCESS;
}

bool SoundManager::CreateSound(const std::string& tag, const std::string& path,
                               SoundType sound_type) {
    if (DoesSoundExist(tag)) {
        ResourceManager::GetInstance().GetSound(tag);

        return FAILED;
    }

    FMOD::Sound* sound;
    if (m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound) != FMOD_OK) {
        /*! Write to logger if debug mode. if not and after this, mute app. */
        std::cerr << "ERROR::CAN::NOT::CREATE::SOUND::" << path << "\n";
        return FAILED;
    }
    else {
        switch (sound_type) {
        case SoundType::EFFECT:
            sound->setMode(FMOD_LOOP_OFF);
            break;
        case SoundType::BACKGROUND:
            //[[fallthrough]]
        case SoundType::SURROUND:
            sound->setMode(FMOD_LOOP_NORMAL);
            sound->setLoopCount(-1);
            break;
        }

        /*! Insert created sound to sound container */
        m_sounds.emplace(std::make_pair(tag, SoundInfo{ sound, sound_type }));
    }

	return SUCCESS;
}

bool SoundManager::CreateSound(const std::string& item_tag) {
    if (DoesSoundExist(item_tag)) {
        return true;
    }
    else {
        auto& sound_path = ResourceManager::GetInstance().GetSound(item_tag);

        FMOD::Sound* sound;
        if (m_system->createSound(sound_path.c_str(), FMOD_DEFAULT, 0, &sound) != FMOD_OK) {
            /*! Write to logger if debug mode. if not and after this, mute app. */
            std::cerr << "ERROR::CAN::NOT::CREATE::SOUND::" << sound_path << "\n";
            return FAILED;
        }

        /*! @todo:Temporary */
        SoundType sound_type{ SoundType::BACKGROUND };

        switch (sound_type) {
        case SoundType::EFFECT:
            sound->setMode(FMOD_LOOP_OFF);
            break;
        case SoundType::BACKGROUND:
            //[[fallthrough]]
        case SoundType::SURROUND:
            sound->setMode(FMOD_LOOP_NORMAL);
            sound->setLoopCount(-1);
            break;
        }

        /*! Insert created sound to sound container */
        m_sounds.emplace(std::make_pair(item_tag, SoundInfo{ sound, sound_type }));
        return SUCCESS;
    }
}

bool SoundManager::DestroySound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
		StopSound(tag);

        //m_sounds.at(tag).m_sound->release();
        m_sounds.erase(tag);
		return SUCCESS;
	}
	else
        return FAILED;
}

void SoundManager::PlaySound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
	    auto result = m_system->playSound(m_sounds.at(tag).Sound(), 0, false, &m_sound_channel);
        if (result != FMOD_OK) {
            /*! Do something */
            std::cerr << "ERROR::CAN::NOT::PLAY::SOUND::" << tag << "\n";
        }
	}
}

void SoundManager::StopSound(const std::string& tag) {
	if (DoesSoundExist(tag)) {
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

void SoundManager::Clear() {
    StopAllSounds();

    /*! Release all sounds */
	for (auto& pair_item : m_sounds) {
		auto& sound = pair_item.second;
        if (sound.Sound()->release() != FMOD_OK) {
            /*! Write to logger if debug mode. if not and after this, mute app. */
            std::cerr << "ERROR::SOUND::RELEASE::FAILED\n";
        }
	}
    m_sounds.clear();
}

void SoundManager::ProcessStopSound(const SoundInfo& sound) {
    //m_system->getChannel()
}
