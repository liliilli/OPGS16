#ifndef OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H
#define OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file System/Boot/Scene/Public/__debugsound.h
/// @log
/// 2018-05-13 Create file.
///

/// ::opgs16::element::CScene
#include <Element\scene.h>

namespace opgs16::builtin::sample {

class __DEBUGSOUND final : public element::CScene {
    void Initiate() override final;
};

} /// opgs16::builtin::sample

#endif /// OPGS16_SYSTEM_BOOT_SCENE_PUBLIC___DEBUGSOUND_H