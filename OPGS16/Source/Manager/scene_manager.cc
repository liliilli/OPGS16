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

#include <Manager\scene_manager.h>      /// Header file

/// ::opgs16::manager::MPhysicsManager
#include <Manager\physics_manager.h>    
/// ::opgs16::manager::MTimerManager
#include <Manager\timer_manager.h>
/// ::opgs16::manager::MTextureMangaer
#include <Manager\texture_manager.h>
/// ::opgs16::manager::MObjectManager
#include <Manager\object_manager.h>
/// ::opgs16::manager::MShaderManager
#include <Manager\shader_manager.h>
/// ::opgs16::manager::MSoundManager
#include <Manager\sound_manager.h>

namespace opgs16::manager {

void MSceneManager::PopScene() {
    entry::SetOnBeforeUpdateCallback(
        std::bind(&MSceneManager::PrivatePopScene, this));
}

void MSceneManager::ReleaseAllResources() const {
    MPhysicsManager::Instance().Clear();  /*! precise */
    MTimerManager::Instance().Clear();    /*! precise */
    MSoundManager::Instance().Clear();    /*! Not precise */
    ShaderManager::Instance().Clear();   /*! Not implemented */
    TextureManager::Instance().Clear();  /*! Not precise? */
    Clear(MObjectManager::Instance());
}

MSceneManager::~MSceneManager() = default;

} /*! opgs16::manager */
