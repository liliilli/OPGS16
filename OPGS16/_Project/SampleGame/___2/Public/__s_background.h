#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_BACKGROUND_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_BACKGROUND_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Boot/Object/___2/Public/__s_background.h
*
* @log
* 2018-04-29 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\object.h> /// ::ogps16::element::CObject
#include <opgs16fwd.h>      /// Forward declaration

namespace opgs16::builtin::sample {

class __S_BACKGROUND final : public element::CObject {
public:
    __S_BACKGROUND();

private:
    component::CPreProcessing2DRenderer* m_renderer{ nullptr };
    void Render() override final;
};

} /*! ::opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_BACKGROUND_H