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
 * @file System/Boot/Scripts/Private/__s_script_manager.cc
 * @brief Implementation of Script file ::opgs16::builtin::sample::__S_SCRIPT_MANAGER
 * @author Jongmin Yun
 * @log
 * 2018-04-14 Create file.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_script_manager.h"               /*! Header file */
#include "../../../Object/___2/Public/__s_game_canvas.h"/*! ::opgs16::builtin::sample::__S_GAME_CANVAS */
#include "../../../Object/___2/Public/__s_player.h"     /*! ::opgs16::builtin::sample::__S_PLAYER */
#include "../../../../Manager/Public/scene_manager.h"   /*! ::opgs16::manager::MSceneManager */

namespace opgs16 {
namespace builtin {
namespace sample {

__S_SCRIPT_MANAGER::__S_SCRIPT_MANAGER(element::CObject& bind_object) : CScriptFrame{ bind_object },
    m_present_scene{ *manager::MSceneManager::Instance().PresentScene() } {
    Start();
};

void __S_SCRIPT_MANAGER::Start() {
    m_ui_object = m_present_scene.Instantiate<__S_GAME_CANVAS>("GameCanvas");
    GetObject().Instantiate<__S_PLAYER>("Player");

    m_status = Status::TITLE;
}

void __S_SCRIPT_MANAGER::Update() {

}

} /*! ogps16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */
