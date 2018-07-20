#ifndef MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_COMPONENT_H
#define MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_COMPONENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

#include <Frame/timer_handle.h>

namespace magiccup {
class ScriptUiBackground;
} /// ::magiccup namespace

namespace magiccup {

class ScriptTitleComponent final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTitleComponent);

  void EnableComponent();

private:
  void Initiate() override final;

  void ChangeBackground();

  magiccup::ScriptUiBackground* m_background = nullptr;
  opgs16::element::CObject* m_subject = nullptr;
  opgs16::element::CObject* m_copyright = nullptr;
  opgs16::element::canvas::CCanvas* m_canvas = nullptr;

  opgs16::element::CTimerHandle m_temporary_timer;
  int32_t m_background_index = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_COMPONENT_H