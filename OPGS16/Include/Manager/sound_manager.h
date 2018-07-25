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

/// ::phitos::enums::ESuccess boolean flag.
#include <Phitos/Enums/success.h>
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
/// @param[in] sound_name The tag to name to sound object.
///
/// @return Success flag.
///
phitos::enums::ESucceed GenerateSoundElement(const std::string& sound_name);

///
/// @brief Destroy sound with tag.
/// @param[in] sound_name The tag to be used for searching sound object.
/// @return The success flag, return true if success.
///
phitos::enums::ESucceed DestroySoundElement(const std::string& sound_name);

///
/// @brief Play specified sound with tag.
/// @param[in] sound_name The tag to find sound object.
///
phitos::enums::ESucceed PlaySoundElement(const std::string& sound_name);

///
/// @brief Stop specified sound with tag.
/// @param[in] sound_name The tag to stop sound object has that tag name.
///
phitos::enums::ESucceed StopSoundElement(const std::string& sound_name);

///
/// @brief
/// Pause given sound channel, all of sounds are paused and stopped.
/// To resume paused sounds, call ResumeSoundChannel() function.
///
bool PauseSoundChannel(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Resume give paused sound channel, all of sounds are resumed and play again.
/// To pause playing sounds, call PauseSoundChannel() function.
///
bool ResumeSoundChannel(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Mute given sound channel, all of sounds are muted.
/// To resume muted sound or check whethere if channel is being muted,
/// call UnmutedSoundChannel() and IsSoundChannelMuted().
///
bool MuteSoundChannel(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Ummute given sound channel, all of sounds are unmuted.
/// To mute sounds or check whether if channel is being muted or not,
/// call MutedSoundChannel() and IsSoundChannelMuted().
///
bool UnmuteSoundChannel(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Pause given master sound channel. all of sounds of all channels are paused.
/// To resume master sound channel, call ResumeMasterSoundChannel();
///
bool PauseMasterSoundChannel();

///
/// @brief
/// Resume given master sound channel. all of sounds of all channels are resume.
/// To pause master sound channel, call PauseMasterSoundChannel();
///
bool ResumeMasterSoundChannel();

///
/// @brief
/// Mute given master sound channel. all of sounds of all channels are muted.
/// To resume master sound channel, call UnmuteMasterSoundChannel();
///
bool MuteMasterSoundChannel();

///
/// @brief
/// Unmute given master sound channel. all of sounds of all channels are unmuted.
/// To mute master sound channel, call MuteMasterSoundChannel();
///
bool UnmuteMasterSoundChannel();

///
/// @brief
/// Set volume of sound channel you speicified.
/// volume_value must be a 0 ~ 100. If not, value will be clamped.
///
void SetVolumeSoundChannel(opgs16::resource::ESoundType sound_channel_type,
                           int32_t volume_value);

///
/// @brief
/// Set volume of master sound channel.
/// volume_value must be a 0 ~ 100. If not, value will be clamped.
///
void SetVolumeMasterChannel(int32_t volume_value);

///
/// @brief
/// Check if or not sound channel you specified is being muted.
///
bool IsSoundChannelMuted(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Check if or not sound channel you specified is being paused.
///
bool IsSoundChannelPaused(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Check if or not master sound channel is being muted.
///
bool IsMasterSoundChannelMuted();

///
/// @brief
/// Check if or not master sound channel is being paused.
///
bool IsMasterSoundChannelPaused();

///
/// @brief Verity whether speicific sound is playing or not.
/// @param[in] sound_name The tag to verify.
///
bool IsSoundElementPlaying(const std::string& sound_name);

///
/// @brief Check if any sound stream with 'sound_name' name is exist.
/// @return If duplicated name is exist already, return true, elsewhere false.
///
bool IsSoundElementExist(const std::string& sound_name);

///
/// @brief
/// Check if sound system is available.
///
bool IsSoundSystemAvailable() noexcept;

///
/// @brief
/// Stop all sound playing of spcified sound channel.
///
void StopSoundChannel(opgs16::resource::ESoundType sound_channel_type);

///
/// @brief
/// Stop all sound playing of spcified master sound channel.
///
void StopMasterSoundChannel();

///
/// @brief Stop all of sounds which are playing.
///
void StopAllSoundElements();

///
/// @brief Release all sounds, but leaves system on active.
///
void ReleaseAllSoundElements();

} /// ::opgs16::manager namespace

#endif /// OPGS16_SYSTEM_MANAGER_SOUND_MANAGER_H
