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
 * @file System/Manager/Private/scene_manager.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-04 Refactoring.
 */

#include "../Public/scene_manager.h"        /*! Header file */

#include "../Public/physics_manager.h"      /*! opgs16::manager::PhysicsManager */
#include "../Public/timer_manager.h"        /*! opgs16::mangaer::TimerManager */
#include "../Public/texture_manager.h"      /*! opgs16::manager::TextureManager */
#include "../Public/object_manager.h"       /*! opgs16::manager::ObjectManager */
#include "../Public/shader_manager.h"       /*! opgs16::manager::ShaderManager */
#include "../Public/sound_manager.h"        /*! opgs16::manager::SoundManager */

namespace opgs16 {
namespace manager {

void SceneManager::PopScene() {
    if (!m_scenes.empty())
        m_scenes.pop();
}

void SceneManager::ReleaseAllResources() const {
    PhysicsManager::Instance().Clear();  /*! precise */
    TimerManager::Instance().Clear();    /*! precise */
    SoundManager::Instance().Clear();    /*! Not precise */
    ShaderManager::Instance().Clear();   /*! Not implemented */
    TextureManager::Instance().Clear();  /*! Not precise? */
    Clear(ObjectManager::Instance());
}

SceneManager::~SceneManager() = default;

} /*! opgs16::manager */
} /*! opgs16 */
