#ifndef OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/sound_manager.h
///
/// @brief Elementary manager class to manage sounds.
/// This file consists of application operation class and member API functions.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-26 Change sound library from OpenAL & alut to Fmod.
/// 2018-03-04 Refactoring.
/// 2018-05-13 Remove m_sound_channel variable, and allocate channel to each sound instead of.
/// 2018-06-15 Refactoring.
///

#include <unordered_map>

/// FMOD sound library
#include <fmod.hpp>
/// ::phitos::enums::EActivated boolean flag.
#include <Phitos/Enums/activated.h>
/// ::phitos::enums::EFound boolean flag.
#include <Phitos/Enums/found.h>
/// ::phitos::enums::ESuccess boolean flag.
#include <Phitos/Enums/success.h>

/// ::phitos::enums::ESwitch switch boolean flag.
#include <Phitos/Enums/switch.h>

/// ::opgs16::manager::_internal namespace
#include <Manager/Internal/sound_info.h>

/// Internal namespace
namespace opgs16::manager::sound::__ {

///
/// @brief
/// Initiate sound setting.
/// This function must not be called once more.
///
void Initiate();

///
/// @brief
/// Shutdown and release all sound resources.
/// This internal function must not be called once more.
///
void Shutdown();

} /// ::opgs16::manager::sound::__ namespace

///
/// @namespace sound
/// @brief This namespace manages rich sounds like a background music, effect sounds,
/// anything related to sound from WAV file, OGG file...
///
/// @bug
/// Although sound mute status is on, sound on playing actually does not stop.
/// Looping is not completely clean. some delay has been created between looped.
///
/// @todo Implement complete sound features.
/// 1. Stop sound feature and pause also.
/// 2. Multiple stimutaneous effect playing.
///
/// @log
/// 2018-02-26 Replace OpenAL with FMOD.
/// 2018-03-04 Refactoring, Move class in opgs16::manager.
/// 2018-05-13 Remove m_sound_channel variable, and allocate channel to each sound instead of.
/// 2018-06-15 Refactoring.
///
namespace opgs16::manager::sound {

///
/// @brief Get sound resource and initialize sound.
/// First, Check sound container if any already sound with same tag exists.
/// and nothing found, get a resource from ResourceManager and initialize.
/// or found sound item with same tag, do nothing and just exit method.
///
/// @param[in] item_tag The tag to name to sound object.
///
/// @return Success flag.
///
phitos::enums::ESucceed GenerateSound(const std::string& item_tag);

///
/// @brief Destroy sound with tag.
/// @param[in] tag The tag to be used for searching sound object.
/// @return The success flag, return true if success.
///
phitos::enums::ESucceed DestroySound(const std::string& tag);

///
/// @brief Play specified sound with tag.
/// @param[in] tag The tag to find sound object.
///
phitos::enums::ESucceed PlaySound(const std::string& tag);

///
/// @brief Stop specified sound with tag.
/// @param[in] tag The tag to stop sound object has that tag name.
///
phitos::enums::ESucceed StopSound(const std::string& tag);

///
/// @brief Verity whether speicific sound is playing or not.
/// @param[in] tag The tag to verify.
///
phitos::enums::EActivated IsSoundPlaying(const std::string& tag);

///
/// @brief Stop all of sounds which are playing.
///
void StopAllSounds();

///
/// @brief Release all sounds, but leaves system on active.
///
void ReleaseAllSounds();

///
/// @brief Check if any sound stream with 'tag' name is exist.
/// @return If duplicated name is exist already, return true, elsewhere false.
///
phitos::enums::EFound IsSoundExist(const std::string& tag);

///
/// @brief Return true or false whenever sound is muted or not.
/// @return If sound is muted, return true, else return false.
///
bool IsSoundMuted() noexcept;

///
/// @brief Set mute with true or false.
/// @param[in] value Mute switch.
///
void SetMute(phitos::enums::ESwitch value);

} /// ::opgs16::manager namespace

#endif /// OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H
