#ifndef OPGS16_SYSTEM_BOOT____2_PUBLIC___S_ENEMY_H
#define OPGS16_SYSTEM_BOOT____2_PUBLIC___S_ENEMY_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Public/__s_enemy.h
 * @brief Enemy object class.
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Element/Public/object.h"  /*! ::ogps16::element::CObject */

namespace opgs16::builtin::sample {

class __S_ENEMY final : public element::CObject {
public:
	__S_ENEMY(CObject* parent);
    CObject* GetParent() {
        return &m_parent;
    }

private:
    CObject& m_parent;
    component::CSprite2DRenderer* m_renderer{ nullptr };

    void Render() override final;
};

} /*! opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT____2_PUBLIC___S_ENEMY_H