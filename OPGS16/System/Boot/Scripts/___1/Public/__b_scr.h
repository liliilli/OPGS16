#ifndef OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H
#define OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H

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
 * @file System/Boot/Scripts/Public/__b_scr.h
 * @author Jongmin Yun
 * @log
 * 2018-04-08 Activate file and add comments.
 * 2018-04-14 Move __B_SCR into ::opgs16::builtin::sample namespace.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Frame/timer_handle.h"            /*! TimerHandle */
#include "../../../../../Headers/script_common.h"      /*! component::ScriptFrame */
#include "../../../../../Headers/Fwd/objectfwd.h"      /*! canvas::Image */

namespace opgs16 {
namespace builtin {
namespace sample {

class __B_SCR final : public component::CScriptFrame {
public:
    __B_SCR(opgs16::element::CObject& obj);
    void Update() override final {};

private:
    TimerHandle m_timer;
    TimerHandle m_timer_2;

    canvas::Image* logo;

private:
    void Start() override final;

    void SetLogoImage();
    void PlaySoundEffect();
    void CreateTextObject();
    void MoveLogo1();
    void MoveLogoSliced();

    void OnTriggerNextScene();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, __B_SCR)
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16::sample */

#endif // OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H