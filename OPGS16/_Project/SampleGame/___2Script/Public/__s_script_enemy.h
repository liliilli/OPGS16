#ifndef OPGS16_SYSTEM_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_ENEMY_H
#define OPGS16_SYSTEM_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_ENEMY_H

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

namespace opgs16::builtin::sample {

class __S_SCRIPT_ENEMY final : public component::CScriptFrame {
public:
    __S_SCRIPT_ENEMY(element::CObject& bind_object);

private:
    void Start() override final {}
    void Update() override final;

    SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, __S_SCRIPT_ENEMY)
};

}

#endif // OPGS16_SYSTEM_BOOT_SCRIPTS____2_PUBLIC___S_SCRIPT_ENEMY_H