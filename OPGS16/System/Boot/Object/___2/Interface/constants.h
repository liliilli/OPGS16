#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_CONSTANTS_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_CONSTANTS_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Interface/constants.h
 * @brief Constants values for using to ___2 sample game.
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

namespace opgs16::builtin::sample {

constexpr float k_end{ -100.f };
constexpr float k_start{ 0.f };
constexpr float k_z_length{ k_start - k_end };
constexpr float k_center_pos[2]{ 128.f, 112.f };
constexpr float k_to_parent_pos[3]{ 0.f, 0.f, k_end };

constexpr float k_rotation_spd{ 3.1275f };
constexpr float k_bullet_speed{ 1.f };

}

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_CONSTANTS_H