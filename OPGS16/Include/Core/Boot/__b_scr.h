#ifndef OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H
#define OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/Boot/__b_scr.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-08 Activate file and add comments.
/// 2018-04-14 Move __B_SCR into ::opgs16::builtin::sample namespace.
///

#include <Frame\timer_handle.h>     /// ::opgs16::element::CTimerHandle
#include <Headers\script_common.h>
#include <opgs16fwd.h>              /// Forward declaration

namespace opgs16::builtin::sample {

class __B_SCR final : public component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(__B_SCR);

private:
  void Start() override final;
  void Update(float delta_time) override final {};
  void Destroy() override final;

  void SetLogoImage();
  void CreateTextObject();
  void MoveLogo1();
  void MoveLogoSliced();

  void OnTriggerNextScene();

  element::CTimerHandle m_timer;
  element::CTimerHandle m_timer_2;

  opgs16::element::canvas::CImage* logo = nullptr;
};

} /// ::opgs16::builtin::sample namespace

#endif /// OPGS16_SYSTEM_BOOT_SCRIPT_PUBLIC___B_SCR_H