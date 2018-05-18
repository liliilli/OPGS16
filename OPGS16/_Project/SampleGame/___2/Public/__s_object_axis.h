#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Public/__s_object_axis.h
 * @brief Object axis file binding all moveable object except for UI objects.
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Element/Public/object.h"  /*! ::ogps16::element::CObject */
#include "../Interface/enum_types.h"            /*! ::opgs16::builtin::sample::ECharacterType */
#include "../Interface/constants.h"
#include "__s_player_bullet.h"

namespace opgs16 {
namespace builtin {
namespace sample {

class __S_OBJECT_AXIS final : public element::CObject {
public:
    template <typename... _Args>
    __S_OBJECT_AXIS(ECharacterType type, _Args... args) {
        static_assert(sizeof...(_Args) > 0, "No parameter pack.");

        switch (type) {
        case ECharacterType::BULLET:
            SetWorldPosition({ k_center_pos[0], k_center_pos[1], k_start });
            m_object = Instantiate<__S_PLAYER_BULLET>("Object", this, std::forward<_Args>(args)...);
            break;
        default: break;
        }
    }

    __S_OBJECT_AXIS(ECharacterType type);

private:
    CObject* m_object{ nullptr };
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! ogps16 */

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H