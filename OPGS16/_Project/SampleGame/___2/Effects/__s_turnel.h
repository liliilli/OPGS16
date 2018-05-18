#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_EFFECTS___S_TURNEL_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_EFFECTS___S_TURNEL_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/Object/___2/Public/__s_turnel.h
 * @author Jongmin Yun
 * @log
 * 2018-04-21 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// ::opgs16::element::CFrameBuferFrame
#include <Shader\framebuffer.h>

namespace opgs16::builtin::sample {

class __S_TURNEL_FRAMEBUFFER final : public element::CFrameBuferFrame {
	void Initialize() override final;
};

} /*! opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_EFFECTS___S_TURNEL_H