
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
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_script_enemy.h" /*! Header file */
#include "../../../Object/___2/Public/__s_enemy.h"
#include "../../../Object/___2/Interface/constants.h"

namespace opgs16::builtin::sample {

__S_SCRIPT_ENEMY::__S_SCRIPT_ENEMY(element::CObject& bind_object) : CScriptFrame{ bind_object } {
    Start();
}

void __S_SCRIPT_ENEMY::Update() {
    using element::_internal::EDirection;
    auto* object = static_cast<__S_ENEMY*>(&GetObject());

    {
        auto m_parent = object->GetParent();

        const float angle_value = m_parent->GetRotationWorldAngle(EDirection::Z) + 0.125f;
        m_parent->SetRotationWorldAngle(EDirection::Z, angle_value);

        auto pos = m_parent->GetWorldPosition();
        pos.z += 0.75f;
        if (pos.z >= 0) pos.z = k_end;
        m_parent->SetWorldPosition(pos);
    }
}

} /*! opgs16::builtin::sample */