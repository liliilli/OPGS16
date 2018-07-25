#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/sound_internal.h
///
/// @brief
/// Internal sound resource and information instance file.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Create file, move internal structure to it.
/// 2018-05-13 Add SSoundInfo FMOD::Channel* variable.

/// Library for sound loading.
#include <fmod.hpp>
#include <Manager/ResourceType/sound.h>

namespace opgs16::manager::_internal {

///
/// @class SSoundResource
/// @brief
/// This class stores sound information.
/// Sound pointer instance, sound type, and sound channel which sound will play
/// through.
///
/// Sound instance and type must be parameterized when creating new SSoundInfo
/// object. Sound channel will be internally created automatically.
///
class SSoundResource {
private:
  using ESoundType = opgs16::resource::ESoundType;

  /// Sound buffer.
  FMOD::Sound*        m_sound = nullptr;
  /// The type of sound.
  const ESoundType    m_type = ESoundType::None;
  /// Channel.
  FMOD::Channel*      m_channel = nullptr;

public:
  SSoundResource() = default;

  explicit SSoundResource(FMOD::Sound* sound, const ESoundType type) :
      m_sound{ sound },
      m_type{ type },
      m_channel{ nullptr }
  {};

  FMOD::Sound* GetSoundBuffer() const noexcept {
    return m_sound;
  }

  ESoundType GetSoundType() const noexcept {
    return m_type;
  }

  FMOD::Channel* GetChannel() const noexcept {
    return m_channel;
  }

  FMOD::Channel** GetChannelPtr() noexcept {
    return &m_channel;
  }
};

} /// ::opgs16::manager::_internal namespace

#endif /// !OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H

