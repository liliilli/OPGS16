#ifndef OPGS16_SYSTEM_BOOT_SCRIPTS_PUBLIC___S_SCRIPT_MANAGER_H
#define OPGS16_SYSTEM_BOOT_SCRIPTS_PUBLIC___S_SCRIPT_MANAGER_H

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
 * @file System/Boot/Scripts/Public/__s_manager.h
 * @brief Script file for ::opgs16::builtin::sample::__S_MANAGER
 * @author Jongmin Yun
 * @log
 * 2018-04-14 Create file.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Components/Public/script_frame.h"    /*! ::opgs16::component::CScriptFrame */
#include "../../../../Frame/timer_handle.h"         /*! CTimerHandle */

namespace opgs16 {
namespace builtin {
namespace sample {

class __S_GAME_CANVAS;

class __S_SCRIPT_MANAGER final : public component::CScriptFrame {
public:
    __S_SCRIPT_MANAGER(element::CObject& bind_object);

private:
    const manager::MInputManager& m_input_manager;

    element::CScene& m_present_scene;   /*! Grasp element::CScene */
    __S_GAME_CANVAS* m_ui_object;

    enum class Status {
        INITIALIZATION, /*! First status */
        TITLE,          /*! Title status */
        STARTING,
        PLAYING,
        PAUSING,
        TIME_OFF,
        GAME_OVER
    };
    Status m_status{ Status::INITIALIZATION };

    uint32_t m_score{ 0 };
    uint32_t m_life{ 0 };
    uint32_t m_stage{ 0 };

    element::CTimerHandle m_common_timer_1;

    void Start() override final;
    void Update() override final;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, __S_SCRIPT_MANAGER)
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! ogps16 */

#endif // OPGS16_SYSTEM_BOOT_SCRIPTS_PUBLIC___S_SCRIPT_MANAGER_H
