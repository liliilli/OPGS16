#ifndef OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H
#define OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Boot/Scene/Public/__debugsound.h
* @log
* 2018-05-13 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../System/Element/Public/scene.h"    /*! Scene */

namespace opgs16::builtin::sample {

class __DEBUGSOUND final : public element::CScene {
    void Initiate() override final;
};

} /*! opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H