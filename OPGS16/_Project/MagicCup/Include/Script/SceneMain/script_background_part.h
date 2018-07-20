#ifndef MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_BACKGROUND_PART_H
#define MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_BACKGROUND_PART_H

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
}

namespace magiccup {

class ScriptBackgroundPart final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptBackgroundPart);

private:
  void Initiate() override final;
  void OnEnabled() override final;
  void OnDisabled() override final;

  void ChangeColorSequencially();

  ScriptUiBackground* m_background = nullptr;

  opgs16::element::CTimerHandle m_timer_handle;
  int32_t m_background_index = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_BACKGROUND_PART_H