
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Private/__s_object_axis.cc
 * @brief Definition file of ../Public/__s_object_axis.h
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_object_axis.h"
#include "../Public/__s_player.h"
#include "../Public/__s_enemy.h"

namespace opgs16::builtin::sample {

__S_OBJECT_AXIS::__S_OBJECT_AXIS(ECharacterType type) {
    switch (type) {
    case ECharacterType::PLAYER:
        SetWorldPosition({ k_center_pos[0], k_center_pos[1], k_start });
        m_object = Instantiate<__S_PLAYER>("Object", this);
        break;
    case ECharacterType::TAKO: case ECharacterType::BIBIBIK:
    case ECharacterType::ZANGGU: case ECharacterType::XIOXI:
        SetWorldPosition({ k_center_pos[0], k_center_pos[1], k_end });
        m_object = Instantiate<__S_ENEMY>("Object", this);
        break;
    default: break;
    }
}

}
