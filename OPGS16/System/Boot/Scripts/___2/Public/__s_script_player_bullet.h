#ifndef OPGS16_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_PLAYER_BULLET_H
#define OPGS16_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_PLAYER_BULLET_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/Scripts/___2/Public/__s_script_player_bullet.h
 * @brief Script file of ::opgs16::builtin::sample::__S_PLAYER_BULLET class.
 * @log
 * 2018-04-15 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Components/Public/script_frame.h" /*! ::opgs16::component::CScriptFrame */

namespace opgs16 {
namespace builtin {
namespace sample {

class __S_SCRIPT_PLAYER_BULLET final : public component::CScriptFrame {
public:
    __S_SCRIPT_PLAYER_BULLET(opgs16::element::CObject& bind_object);

private:
    void Start() override final;
    void Update() override final;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, __S_SCRIPT_PLAYER_BULLET)
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */

#endif // OPGS16_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_PLAYER_BULLET_H