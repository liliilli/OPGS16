#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/sound_manager.cc
///
/// @brief Sound manager implementation file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Refactoring.
/// 2018-05-13 Changed std::cerr to PUSH_LOG_ERRO logger routine.
/// 2018-06-15 Refactoring.
///
/// @todo Make sound mute system.
///

/// Header file
#include <Manager/sound_manager.h>

#include <string>

/// Enhanced assertion.
#include <Phitos/Dbg/assert.h>
/// ::phitos::enums::EInitiated
#include <Phitos/Enums/initiated.h>
/// import logger
#include <Headers/import_logger.h>
/// ::opgs16::manager::MResourceManager
#include <Manager/resource_manager.h>
///
#include <Manager/resource_type.h>

//!
//! Datas
//!

namespace {

using phitos::enums::EInitiated;
using TSoundContainerMap =
    std::unordered_map<std::string, opgs16::manager::_internal::SSoundInfo>;

EInitiated m_initiated = EInitiated::NotInitiated;

FMOD::System*   m_system = nullptr;
unsigned        m_version = 0;
int             m_sound_driver_count = 0;

// Sound container stores all of sound to be played in application.
TSoundContainerMap m_sounds = {};

// Switch if all sounds needs to be sliented.
phitos::enums::ESwitch m_is_muted = phitos::enums::ESwitch::Off;

} /// unnamed namespace

//!
//! Definition
//!

namespace {

///
/// @brief
/// This private method actually processes stopping sound be playing.
/// This must be called StopSound method and StopAllSounds method.
///
/// @param[in] sound The sound container to stop.
///
void ProcessStopSound(const opgs16::manager::_internal::SSoundInfo& sound) {
  auto result = sound.Channel()->stop();
}

} /// unnamed namespace

namespace opgs16::manager::sound::__ {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated sound::__::Initiated() function call is prohibited.");

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
  m_is_muted = phitos::enums::ESwitch::On;
  PHITOS_NOT_IMPLEMENTED_ASSERT();

label_success:
  return;
}

void Shutdown() {
  StopAllSounds();

  // Release all sounds
  for (auto& pair_item : m_sounds) {
    auto& sound = pair_item.second;
    if (sound.Sound()->release() != FMOD_OK) {
      // Write to logger if debug mode. if not and after this, mute app.
      PUSH_LOG_ERRO("ERROR::SOUND::RELEASE::FAILED\n");
    }
  }
  m_sounds.clear();

  // Close and release sound system
  m_system->close();
  m_system->release();
}

} /// ::opgs16::manager::sound::__ namespace.

namespace opgs16::manager::sound {

using phitos::enums::EActivated;
using phitos::enums::EFound;
using phitos::enums::ESucceed;

using _internal::ESoundType;
using _internal::SSoundInfo;

ESucceed GenerateSound(const std::string& item_tag) {
  if (IsSoundExist(item_tag) == EFound::Found) {
    PUSH_LOG_INFO_EXT("Redundant GenerateSound() call. [Sound item tag : {}]",
        item_tag);
    return ESucceed::Succeed;
  }

  auto sound_item = resource::GetSound(item_tag);

  FMOD::Sound* sound = nullptr;
  if (const auto result = m_system->createSound(sound_item->Path().c_str(),
          FMOD_DEFAULT, 0, &sound);
      result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT(
        "Could not create sound. [Sound item tag : {}], [Sound item path : {}]",
        item_tag, sound_item->Path());
    return ESucceed::Failed;
  }

  ESoundType sound_type = ESoundType::Effect;
  if (sound_item->IsBgm())
    sound_type = ESoundType::Background;

  switch (sound_type) {
  case ESoundType::Effect:
    sound->setMode(FMOD_LOOP_OFF);
    break;
  case ESoundType::Background:
  case ESoundType::Surround:
    sound->setMode(FMOD_LOOP_NORMAL);
    sound->setLoopCount(-1);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  // Insert created sound to sound container.
  m_sounds.emplace(std::make_pair(item_tag, SSoundInfo{ sound, sound_type }));
  return ESucceed::Succeed;
}

ESucceed DestroySound(const std::string& tag) {
  if (IsSoundExist(tag) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("Failed to find sound, {}, [{} : {}]",
        "in DestroySound().", "Sound name", tag);
    return ESucceed::Failed;
  }

  StopSound(tag);

  const auto result = m_sounds[tag].Sound()->release();
  if (result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT("Failed to delete sound incompletely. [{} : {}]",
        "Sound name", tag);
    m_sounds.erase(tag);
    return ESucceed::Failed;
  }

  m_sounds.erase(tag);
  PUSH_LOG_INFO_EXT("Deleted sound resource. [{} : {}]", "Sound name", tag);
  return ESucceed::Succeed;
}

ESucceed PlaySound(const std::string& tag) {
  if (IsSoundExist(tag) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("Failed to find sound, {}, [{} : {}]",
        "in PlaySound().", "Sound name", tag);
    return ESucceed::Failed;
  }

  const auto result = m_system->playSound(m_sounds[tag].Sound(), 0, false,
                                          m_sounds[tag].ChannelPtr());
  if (result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT("Failed to play sound. [{} : {}]", "Sound name", tag);
    StopSound(tag);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed StopSound(const std::string& tag) {
  if (IsSoundExist(tag) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("Failed to stop sound. [{} : {}]", "Sound name", tag);
    return ESucceed::Failed;
  }

  ProcessStopSound(m_sounds[tag]);
  return ESucceed::Succeed;
}

EActivated IsSoundPlaying(const std::string& tag) {
  if (IsSoundExist(tag) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("Failed to find sound, {}. [{} : {}]",
        "in IsSoundPlaying()", "Sound name", tag);
    return EActivated::Disabled;
  }

  bool is_playing = false;
  m_sounds[tag].Channel()->isPlaying(&is_playing);

  return is_playing ? EActivated::Activated : EActivated::Disabled;
}

void StopAllSounds() {
  for (auto& pair_item : m_sounds) {
    auto& sound = pair_item.second;
    ProcessStopSound(sound);
  }
}

void ReleaseAllSounds() {
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

EFound IsSoundExist(const std::string& tag) {
	return m_sounds.find(tag) != m_sounds.end() ? EFound::Found : EFound::NotFound;
}

bool IsSoundMuted() noexcept {
  return static_cast<bool>(m_is_muted);
}

void SetMute(phitos::enums::ESwitch value) {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  m_is_muted = value;
}

} /// ::opgs16::manager namespace

