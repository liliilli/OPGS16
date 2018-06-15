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

namespace opgs16::manager::_internal {

///
/// @enum ESoundType
/// @brief
/// ESoundType is type of each sounds to have been storing.
///
enum class ESoundType {
  Effect,		/** This is for effect sound, once play but not looped normally. */
  Background, /** This is for background sound, looped normally. */
  Surround,   /** This is for 3D surround ambient sound have distance. */
  Nothing     // Default Initailized type.
};

///
/// @class SSoundInfo
/// @brief
/// This class stores sound information.
/// Sound pointer instance, sound type, and sound channel which sound will play
/// through.
///
/// Sound instance and type must be parameterized when creating new SSoundInfo
/// object. Sound channel will be internally created automatically.
///
class SSoundInfo {
private:
  FMOD::Sound*        m_sound;        /*! Sound buffer */
  const ESoundType    m_type;	        /*! The type of sound. */
  FMOD::Channel*      m_channel;      /*! Channel */

public:
  SSoundInfo() : m_type{ ESoundType::Nothing } {};
  explicit SSoundInfo(FMOD::Sound* sound, const ESoundType type) :
    m_sound{ sound }, m_type{ type }, m_channel{ nullptr } {};

  FMOD::Sound*    Sound() const noexcept { return m_sound; }
  ESoundType      Type() const noexcept { return m_type; }
  FMOD::Channel*  Channel() const noexcept { return m_channel; }
  FMOD::Channel** ChannelPtr() noexcept { return &m_channel; }
};

} /// ::opgs16::manager::_internal namespace

#endif /// !OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H

