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
 * @file System/Boot/Scripts/___2/Private/__s_script_player.cc
 * @brief Definition file of ../Public/__s_script_player.h
 * @author Jongmin Yun
 * @log
 * 2018-04-15 Create file.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_script_player.h"                /*! Header file */
#include "../../../../Manager/Public/input_manager.h"   /*! ::opgs16::manager::MInputManager */
#include "../../../../Manager/Public/scene_manager.h"   /*! ::opgs16::manager::MSceneManager */
#include "../../../../Element/Public/object.h"          /*! ::opgs16::element::CObject */
#include "../../../Object/___2/Public/__s_player.h"
#include "../../../Object/___2/Interface/helper_function.h"
#include "../../../Object/___2/Interface/constants.h"

#include "../../../../../Headers/import_logger.h"

namespace opgs16 {
namespace builtin {
namespace sample {

__S_SCRIPT_PLAYER::__S_SCRIPT_PLAYER(element::CObject& bind_object) :
    CScriptFrame{ bind_object }, m_input_manager{ GET_INPUT_MANAGER },
    m_game_scene{ *manager::MSceneManager::Instance().PresentScene() } {
}

void __S_SCRIPT_PLAYER::Start() {
}

void __S_SCRIPT_PLAYER::Update() {
    using element::_internal::EDirection;

    if (m_input_manager.IsKeyPressed("KeyD")) {
        auto& obj = GetObject();
        const auto& pos = obj.GetWorldPosition();

        const float z_angle = obj.GetRotationFromParentAngle(EDirection::Z);
        GenerateCharacterObject(&m_game_scene, "PBullet", ECharacterType::BULLET, pos.x, pos.y, pos.z, z_angle);
    }

    if (const auto h_val = m_input_manager.GetKeyValue("Horizontal"); h_val) {
        auto* object = static_cast<__S_PLAYER*>(&GetObject());
        auto m_parent = object->GetParent();

        float angle_value = m_parent->GetRotationWorldAngle(EDirection::Z);
        angle_value += k_rotation_spd * h_val;
        m_parent->SetRotationWorldAngle(EDirection::Z, angle_value);
    }
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */