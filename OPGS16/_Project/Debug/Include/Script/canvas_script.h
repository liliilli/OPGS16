#ifndef OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H
#define OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Scripts/___3/Public/canvas_script.h
 * @log
 * 2018-05-13 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// ::opgs16::component::CScriptFrame */
#include <Component\script_frame.h>
/// Forward declaration
#include <opgs16fwd.h>

namespace opgs16::builtin::sample {

class DebugCanvasScript final : public component::CScriptFrame {
public:
    DebugCanvasScript(opgs16::element::CObject& bind_object);

private:
    void Initiate() override final;
    void Start() override final;
    void Update() override final;

    void Input();

    manager::MInputManager* m_input = nullptr;

    element::canvas::CText* m_bgm_sentence = nullptr;
    element::canvas::CText* m_info_sentence = nullptr;

    bool    m_is_play_bgm = false;
    int16_t m_mode = 0;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, DebugCanvasScript)
};

} /*! opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H