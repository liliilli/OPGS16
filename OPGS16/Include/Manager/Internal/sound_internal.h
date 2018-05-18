#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H

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
 * @file System/Manager/Internal/sound_internal.h
 * @brief
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Create file, move internal structure to it.
 * 2018-05-13 Add SSoundInfo FMOD::Channel* variable.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <fmod.hpp>

namespace opgs16 {
namespace manager {
namespace _internal {

/*! 
 * @enum ESoundType
 * @brief
 * ESoundType is type of each sounds to have been storing. 
 */
enum class ESoundType {
    EFFECT,		/** This is for effect sound, once play but not looped normally. */
    BACKGROUND, /** This is for background sound, looped normally. */
    SURROUND,   /** This is for 3D surround ambient sound have distance. */
    NOTHING     // Default Initailized type.
};

/*! 
 * @class SSoundInfo
 * @brief 
 * This class stores sound information.
 * Sound pointer instance, sound type, and sound channel which sound will play
 * through.
 *
 * Sound instance and type must be parameterized when creating new SSoundInfo
 * object. Sound channel will be internally created automatically.
 */
class SSoundInfo {
private:
    FMOD::Sound*        m_sound;        /*! Sound buffer */
    const ESoundType    m_type;	        /*! The type of sound. */
    FMOD::Channel*      m_channel;      /*! Channel */

public:
    SSoundInfo() : m_type { ESoundType::NOTHING } {};
    explicit SSoundInfo(FMOD::Sound* sound, const ESoundType type) :
        m_sound{ sound }, m_type{ type }, m_channel{ nullptr } {};

    FMOD::Sound*    Sound() const { return m_sound; }
    ESoundType      Type() const { return m_type; }
    FMOD::Channel*  Channel() const { return m_channel; }
    FMOD::Channel** ChannelPtr() { return &m_channel; }
};

} /*! opgs16::manager::_internal */
} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_INTERNAL_SOUND_INTERNAL_H

