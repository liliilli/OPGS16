/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/Scripts/___2/Private/__s_script_player_bullet.cc
 * @brief Definition file of ../Public/__s_script_player_bullet.h
 * @log
 * 2018-04-15 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_script_player_bullet.h"         /*! Header file */
#include "../../../../Element/Public/object.h"          /*! ::opgs16::element::CObject */
#include "../../../../Manager/Public/object_manager.h"  /*! ::opgs16::manager::MObjectManager */
#include "../../../Object/___2/Public/__s_player_bullet.h"

namespace opgs16 {
namespace builtin {
namespace sample {

constexpr float center_point[3]{ 0.f, 0.f, -100.f };
static float diff_speed[3]{ 0.f, 0.f, 0.f };

__S_SCRIPT_PLAYER_BULLET::__S_SCRIPT_PLAYER_BULLET(element::CObject& bind_object) : CScriptFrame{ bind_object } {
    Start();
    auto init_pos = GetObject().GetWorldPosition();

    const float diff_x = center_point[0] - init_pos.x;
    const float diff_y = center_point[1] - init_pos.y;
    const float diff_z = center_point[2] - init_pos.z;

    const float length = sqrtf(powf(diff_x, 2) + powf(diff_y, 2) + powf(diff_z, 2));
    diff_speed[0] = diff_x / length * 3;
    diff_speed[1] = diff_y / length * 3;
    diff_speed[2] = diff_z / length * 3;
};

void __S_SCRIPT_PLAYER_BULLET::Start() {

};

void __S_SCRIPT_PLAYER_BULLET::Update() {
    auto pos = GetObject().GetWorldPosition();
    pos.z += diff_speed[2];

    if (pos.z <= -100)
        manager::MObjectManager::Instance().Destroy(*static_cast<__S_PLAYER_BULLET*>(&GetObject())->GetParent());
    else
        GetObject().SetWorldPosition(pos);
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */
