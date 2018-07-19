#ifndef MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_SELECT_H
#define MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {

class ScriptTitleDisplay final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTitleDisplay);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  void ShowUpComponents();

  opgs16::element::CObject* m_title = nullptr;

  opgs16::element::CTimerHandle m_timer;

  float m_elapsed = 0.f;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_SELECT_H