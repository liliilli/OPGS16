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
/// 2018-07-25 Make channel system.
///

/// Header file
#include <Manager/sound_manager.h>

#include <string>
#include <unordered_map>

/// FMOD sound library
#include <fmod.hpp>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>

#include <Headers/import_logger.h>
#include <Manager/resource_manager.h>
#include <Manager/ResourceType/sound.h>

/// ::phitos::enums::EFound boolean flag.
#include <Phitos/Enums/found.h>

//!
//! Datas
//!

namespace {
using phitos::enums::EInitiated;
using opgs16::manager::_internal::SSoundResource;

template <typename TType>
using TStringHashMap = std::unordered_map<std::string, TType>;
using TSoundContainerMap = TStringHashMap<SSoundResource>;
using TSoundChannelGroup = TStringHashMap<FMOD::ChannelGroup*>;

inline constexpr const char* err_failed_create_sound_system =
"ERROR::DID::NOT::CREATE::SOUND::SYSTEM";
inline constexpr const char* err_duplicated_initiate_call =
"Duplicated sound::__::Initiated() function call is prohibited.";
inline constexpr const char* err_version_mismatch =
"ERROR::DOES::NOT::MATCH::FMOD::LIBRARY::VERSION";
inline constexpr const char* err_failed_to_get_drivers =
"ERROR::NOT::FOUND::SOUND::DEVICE";
inline constexpr const char* err_failed_to_initialize =
"ERROR::COULD::NOT::CREATE::SOUND::CHANNEL";
inline constexpr const char* err_failed_to_get_master_channel =
"Failed to retrieve information of master channel.";
inline constexpr const char* err_failed_to_get_each_channel =
"ERROR::COULD::NOT::CREATE::SOUND::CHANNEL";
inline constexpr const char* err_failed_to_find_information =
"Failed to find sound information.";

inline constexpr const char* log_item_already_exist =
"Redundant GenerateSoundElement() call. [Sound item tag : {}]";
inline constexpr const char* log_failed_to_create_sound =
"Could not create sound. [Sound item tag : {}], [Sound item path : {}]";
inline constexpr const char* log_failed_to_release_sound =
"ERROR::SOUND::RELEASE::FAILED";

inline constexpr const char* s_channel_effect     = "opEffect";
inline constexpr const char* s_channel_background = "opBack";

EInitiated m_initiated = EInitiated::NotInitiated;

FMOD::System*   s_sound_system = nullptr;
unsigned        m_version = 0;
int             m_sound_driver_count = 0;

// Switch if all sounds needs to be sliented.
bool s_is_sound_system_available = false;

// Sound container stores all of sound to be played in application.
TSoundContainerMap m_sounds;

//TSoundChannelGroup s_channel_group;
FMOD::ChannelGroup* s_master_channel = nullptr;
FMOD::ChannelGroup* s_effect_channel = nullptr;
FMOD::ChannelGroup* s_background_channel = nullptr;

} /// unnamed namespace

//!
//! Definition
//!

namespace {

///
/// @brief
/// This private method actually processes stopping sound be playing.
/// This must be called StopSoundElement method and StopAllSoundElements method.
///
/// @param[in] sound The sound container to stop.
///
void ProcessStopSound(const SSoundResource& sound) {
  sound.GetChannel()->stop();
}

} /// unnamed namespace

namespace opgs16::manager::sound::__ {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
                err_duplicated_initiate_call);

  if (FMOD::System_Create(&s_sound_system) != FMOD_OK) {
    PUSH_LOG_ERRO(err_failed_create_sound_system);
    s_sound_system = nullptr;
    return;
  }

  if (s_sound_system->getVersion(&m_version); m_version < FMOD_VERSION) {
    PUSH_LOG_ERRO(err_version_mismatch);
    s_sound_system = nullptr;
    return;
  }

  if (s_sound_system->getNumDrivers(&m_sound_driver_count);
      m_sound_driver_count <= 0) {
    PUSH_LOG_ERRO(err_failed_to_get_drivers);
    s_sound_system = nullptr;
    return;
  }

  if (s_sound_system->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) {
    PUSH_LOG_ERRO(err_failed_to_initialize);
    s_sound_system = nullptr;
    return;
  }

  if (s_sound_system->createChannelGroup(
          s_channel_effect,
          &s_effect_channel) != FMOD_OK) {
    PUSH_LOG_ERRO(err_failed_to_get_each_channel);
    s_effect_channel = nullptr;
    return;
  }

  if (s_sound_system->createChannelGroup(
          s_channel_background,
          &s_background_channel) != FMOD_OK) {
    PUSH_LOG_ERRO(err_failed_to_get_each_channel);
    s_background_channel = nullptr;
    return;
  }

  if (s_sound_system->getMasterChannelGroup(&s_master_channel) != FMOD_OK) {
    PUSH_LOG_ERRO(err_failed_to_get_master_channel);
    return;
  }

  if (s_master_channel->addGroup(s_effect_channel) != FMOD_OK) {
    return;
  }

  if (s_master_channel->addGroup(s_background_channel) != FMOD_OK) {
    return;
  }

  s_is_sound_system_available = true;
}

void Shutdown() {
  // Release all sound channels
  if (s_effect_channel) {
    s_effect_channel->stop();
    s_effect_channel->release();
  }

  if (s_background_channel) {
    s_background_channel->stop();
    s_background_channel->release();
  }

  // Release all sound resources
  StopAllSoundElements();
  ReleaseAllSoundElements();
  m_sounds.clear();

  s_sound_system->close();
  s_sound_system->release();
}

} /// ::opgs16::manager::sound::__ namespace.

namespace opgs16::manager::sound {
using phitos::enums::ESucceed;
using opgs16::resource::ESoundType;
using _internal::SSoundResource;

ESucceed GenerateSoundElement(const std::string& sound_name) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return ESucceed::Failed;
  }

  if (!IsSoundElementExist(sound_name)) {
    PUSH_LOG_INFO_EXT(log_item_already_exist, sound_name);
    return ESucceed::Succeed;
  }

  // Get sound
  auto* sound_info = resource::GetSound(sound_name);
  if (!sound_info) {
    PHITOS_ASSERT(sound_info, err_failed_to_find_information);
    return ESucceed::Failed;
  }

  // Create sound resource
  FMOD::Sound* sound = nullptr;
  const auto result = s_sound_system->createSound(
      sound_info->GetPath().c_str(),
      FMOD_DEFAULT, 0, &sound);

  if (result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT(log_failed_to_create_sound, sound_name, sound_info->GetPath());
    return ESucceed::Failed;
  }

  // Set ESoundType of created sound resource
  ESoundType sound_type = ESoundType::None;
  switch (sound_info->GetSoundType()) {
  case ESoundType::BackgroundMusic:
    sound_type = ESoundType::BackgroundMusic;
    sound->setMode(FMOD_LOOP_NORMAL);
    sound->setLoopCount(-1);
    break;
  case ESoundType::Effect:
    sound_type = ESoundType::Effect;
    sound->setMode(FMOD_LOOP_OFF);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  // Insert created sound to sound container.
  m_sounds.try_emplace(sound_name, SSoundResource{ sound, sound_type });
  return ESucceed::Succeed;
}

ESucceed DestroySoundElement(const std::string& sound_name) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return ESucceed::Failed;
  }

  if (!IsSoundElementExist(sound_name)) {
    PUSH_LOG_ERROR_EXT(
      "Failed to find sound in DestroySoundElement(), [Sound name : {}]", sound_name);
    return ESucceed::Failed;
  }

  StopSoundElement(sound_name);

  // Release speicified sound resources
  const auto result = m_sounds[sound_name].GetSoundBuffer()->release();
  if (result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT("Failed to delete sound incompletely. [{} : {}]", "Sound name", sound_name);
    m_sounds.erase(sound_name);
    return ESucceed::Failed;
  }

  m_sounds.erase(sound_name);
  PUSH_LOG_INFO_EXT("Deleted sound resource. [{} : {}]", "Sound name", sound_name);
  return ESucceed::Succeed;
}

ESucceed PlaySoundElement(const std::string& sound_name) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return ESucceed::Failed;
  }

  if (!IsSoundElementExist(sound_name)) {
    PUSH_LOG_ERROR_EXT("Failed to find sound, {}, [{} : {}]", "in PlaySoundElement().", "Sound name", sound_name);
    return ESucceed::Failed;
  }

  // If speicifed sound is found, play sound along with ESoundType of this.
  const auto result = s_sound_system->playSound(
      m_sounds[sound_name].GetSoundBuffer(),
      0, false,
      m_sounds[sound_name].GetChannelPtr());

  if (result != FMOD_OK) {
    PUSH_LOG_ERROR_EXT("Failed to play sound. [{} : {}]", "Sound name", sound_name);
    StopSoundElement(sound_name);
    return ESucceed::Failed;
  }

  // And bind instance to appropriate sound channel.
  switch (m_sounds[sound_name].GetSoundType()) {
  case ESoundType::Effect:
    if (s_effect_channel) {
      m_sounds[sound_name].GetChannel()->setChannelGroup(s_effect_channel);
    }
    break;
  case ESoundType::BackgroundMusic:
    if (s_background_channel) {
      m_sounds[sound_name].GetChannel()->setChannelGroup(s_background_channel);
    }
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  return ESucceed::Succeed;
}

ESucceed StopSoundElement(const std::string& sound_name) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return ESucceed::Failed;
  }

  if (!IsSoundElementExist(sound_name)) {
    PUSH_LOG_ERROR_EXT("Failed to stop sound. [{} : {}]", "Sound name", sound_name);
    return ESucceed::Failed;
  }

  ProcessStopSound(m_sounds[sound_name]);
  return ESucceed::Succeed;
}

void ReleaseAllSoundElements() {
  StopAllSoundElements();

  // Release all sounds
  for (auto& pair_item : m_sounds) {
    auto& sound = pair_item.second;
    if (sound.GetSoundBuffer()->release() != FMOD_OK) {
      PUSH_LOG_ERRO("ERROR::SOUND::RELEASE::FAILED\n");
    }
  }
  m_sounds.clear();
}

bool PauseSoundChannel(ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH(); return false;
  case ESoundType::Effect: {
    if (s_effect_channel) {
      bool is_paused = false;
      if (s_effect_channel->getPaused(&is_paused); is_paused) {
        PUSH_LOG_DEBUG("Effect sound channel already paused.");
        return false;
      }

      s_effect_channel->setPaused(true);
      PUSH_LOG_DEBUG("Effect sound channel has paused.");
      return true;
    }
  } break;
  case ESoundType::BackgroundMusic: {
    if (s_background_channel) {
      bool is_paused = false;
      if (s_background_channel->getPaused(&is_paused); is_paused) {
        PUSH_LOG_DEBUG("Effect sound channel already paused.");
        return false;
      }

      s_background_channel->setPaused(true);
      PUSH_LOG_DEBUG("Background sound channel has paused.");
      return true;
    }
  } break;
  }

  return false;
}

bool ResumeSoundChannel(ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH(); return false;
  case ESoundType::Effect: {
    if (s_effect_channel) {
      bool is_paused = true;
      if (s_effect_channel->getPaused(&is_paused); !is_paused) {
        PUSH_LOG_DEBUG("Effect sound channel already resumed.");
        return false;
      }

      s_effect_channel->setPaused(false);
      PUSH_LOG_DEBUG("Effect sound channel has resumed.");
      return true;
    }
  } break;
  case ESoundType::BackgroundMusic: {
    if (s_background_channel) {
      bool is_paused = true;
      if (s_background_channel->getPaused(&is_paused); !is_paused) {
        PUSH_LOG_DEBUG("Effect sound channel already resumed.");
        return false;
      }

      s_background_channel->setPaused(false);
      PUSH_LOG_DEBUG("Background sound channel has resumed.");
      return true;
    }
  } break;
  }

  return false;
}

bool MuteSoundChannel(ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH();
  case ESoundType::Effect: {
    if (s_effect_channel) {
      bool is_muted = false;
      if (s_effect_channel->getMute(&is_muted); is_muted) {
        PUSH_LOG_DEBUG("Effect sound channel already muted.");
        return false;
      }

      s_effect_channel->setMute(true);
      PUSH_LOG_DEBUG("Effect sound channel has muted.");
      return true;
    }
  } break;
  case ESoundType::BackgroundMusic: {
    if (s_background_channel) {
      bool is_muted = false;
      if (s_background_channel->getMute(&is_muted); is_muted) {
        PUSH_LOG_DEBUG("Effect sound channel already muted.");
        return false;
      }

      s_background_channel->setMute(true);
      PUSH_LOG_DEBUG("Background sound channel has muted.");
      return true;
    }
  } break;
  }

  return false;
}

bool UnmuteSoundChannel(ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case ESoundType::Effect: {
    if (s_effect_channel) {
      bool is_muted = true;
      if (s_effect_channel->getMute(&is_muted); !is_muted) {
        PUSH_LOG_DEBUG("Effect sound channel already unmuted.");
        return false;
      }

      s_effect_channel->setMute(false);
      PUSH_LOG_DEBUG("Effect sound channel has unmuted.");
      return true;
    }
  } break;
  case ESoundType::BackgroundMusic: {
    if (s_background_channel) {
      bool is_muted = true;
      if (s_background_channel->getMute(&is_muted); !is_muted) {
        PUSH_LOG_DEBUG("Effect sound channel already unmuted.");
        return false;
      }

      s_background_channel->setMute(false);
      PUSH_LOG_DEBUG("Background sound channel has unmuted.");
      return true;
    }
  } break;
  }

  return false;
}

bool PauseMasterSoundChannel() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  if (s_master_channel) {
    bool is_paused = false;
    if (s_master_channel->getPaused(&is_paused); is_paused) {
      PUSH_LOG_DEBUG("Master sound channel already paused.");
      return false;
    }

    s_master_channel->setPaused(true);
    PUSH_LOG_DEBUG("Master sound channel has paused.");
    return true;
  }

  return false;
}

bool ResumeMasterSoundChannel() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  if (s_master_channel) {
    bool is_paused = true;
    if (s_master_channel->getPaused(&is_paused); !is_paused) {
      PUSH_LOG_DEBUG("Master sound channel already resumed.");
      return false;
    }

    s_master_channel->setPaused(false);
    PUSH_LOG_DEBUG("Master sound channel has resumed.");
    return true;
  }

  return false;
}

bool MuteMasterSoundChannel() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  if (s_master_channel) {
    bool is_muted = false;
    if (s_master_channel->getMute(&is_muted); is_muted) {
      PUSH_LOG_DEBUG("Master sound channel already muted.");
      return false;
    }

    s_master_channel->setMute(true);
    PUSH_LOG_DEBUG("Master sound channel has muted.");
    return true;
  }

  return false;
}

bool UnmuteMasterSoundChannel() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return false;
  }

  if (s_master_channel) {
    bool is_muted = true;
    if (s_master_channel->getMute(&is_muted); !is_muted) {
      PUSH_LOG_DEBUG("Master sound channel already unmuted.");
      return false;
    }

    s_master_channel->setMute(false);
    PUSH_LOG_DEBUG("Master sound channel has unmuted.");
    return true;
  }

  return false;
}

void SetVolumeSoundChannel(opgs16::resource::ESoundType sound_channel_type,
                           int32_t volume_value) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }

  if (volume_value > 100) volume_value = 100;
  if (volume_value < 0)   volume_value = 0;
  const auto volume = static_cast<float>(volume_value) / 100;

  switch (sound_channel_type) {
  case ESoundType::Effect:
    if (s_effect_channel) {
      s_effect_channel->setVolume(volume);
    } break;
  case ESoundType::BackgroundMusic:
    if (s_background_channel) {
      s_background_channel->setVolume(volume);
    } break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

void SetVolumeMasterChannel(int32_t volume_value) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }

  if (volume_value > 100) volume_value = 100;
  if (volume_value < 0)   volume_value = 0;
  const auto volume = static_cast<float>(volume_value) / 100;

  if (!s_master_channel) {
    PUSH_LOG_ERRO("Could not set volume of master sound channel. Channel may not be initialized.");
    return;
  }

  s_master_channel->setVolume(volume);
}

bool IsSoundChannelMuted(opgs16::resource::ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }

  bool is_muted = false;

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case ESoundType::Effect:
    if (!s_effect_channel) {
      PUSH_LOG_ERRO("Could not get mute state of effect channel. Effect sound channel may not be initialized.");
      return false;
    }

    s_effect_channel->getMute(&is_muted);
    return is_muted;
  case ESoundType::BackgroundMusic:
    if (!s_background_channel) {
      PUSH_LOG_ERRO("Could not get mute state of background channel. Background sound channel may not be initialized.");
      return false;
    }

    s_background_channel->getMute(&is_muted);
    return is_muted;
  }

  return false;
}

bool IsSoundChannelPaused(opgs16::resource::ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }

  bool is_paused = false;

  switch (sound_channel_type) {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case ESoundType::Effect:
    if (!s_effect_channel) {
      PUSH_LOG_ERRO("Could not get pause state of effect channel. Effect sound channel may not be initialized.");
      return false;
    }

    s_effect_channel->getPaused(&is_paused);
    return is_paused;
  case ESoundType::BackgroundMusic:
    if (!s_background_channel) {
      PUSH_LOG_ERRO("Could not get pause state of background channel. Background sound channel may not be initialized.");
      return false;
    }

    s_background_channel->getPaused(&is_paused);
    return is_paused;
  }

  return false;
}

bool IsMasterSoundChannelMuted() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }
  if (!s_master_channel) {
    PUSH_LOG_ERRO("Could not get mute state of master sound channel. Channel may not be initialized.");
    return false;
  }

  bool is_muted = false;
  s_master_channel->getMute(&is_muted);
  return is_muted;
}

bool IsMasterSoundChannelPaused() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }
  if (!s_master_channel) {
    PUSH_LOG_ERRO("Could not get mute state of master sound channel. Channel may not be initialized.");
    return false;
  }

  bool is_paused = false;
  s_master_channel->getPaused(&is_paused);
  return is_paused;
}

bool IsSoundElementPlaying(const std::string& sound_name) {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }
  if (!IsSoundElementExist(sound_name)) {
    PUSH_LOG_ERROR_EXT("Failed to find sound in IsSoundElementPlaying(). [{} : {}]", "Sound name", sound_name);
    return false;
  }

  bool is_playing = false;
  m_sounds[sound_name].GetChannel()->isPlaying(&is_playing);
  return is_playing;
}

void StopSoundChannel(opgs16::resource::ESoundType sound_channel_type) {
  PHITOS_ASSERT(sound_channel_type != ESoundType::None, "Sound channel type must not be None.");
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }

  switch (sound_channel_type) {
  case ESoundType::Effect:
    if (s_effect_channel) {
      s_effect_channel->stop();
    } break;
  case ESoundType::BackgroundMusic:
    if (s_background_channel) {
      s_background_channel->stop();
    } break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

void StopMasterSoundChannel() {
  if (!IsSoundSystemAvailable()) {
    PUSH_LOG_WARN("Sound system is not available.");
    return;
  }
  if (!s_master_channel) {
    PUSH_LOG_ERRO("Could not set volume of master sound channel. Channel may not be initialized.");
    return;
  }

  s_master_channel->stop();
}

void StopAllSoundElements() {
  for (auto& pair_item : m_sounds) {
    auto& sound = pair_item.second;
    ProcessStopSound(sound);
  }
}

bool IsSoundElementExist(const std::string& sound_name) {
	return m_sounds.find(sound_name) != m_sounds.end();
}

bool IsSoundSystemAvailable() noexcept {
  return s_is_sound_system_available;
}

} /// ::opgs16::manager namespace

