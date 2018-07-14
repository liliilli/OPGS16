#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {
class ScriptObjectManagement;
} /// Forward declaration

namespace magiccup {

class ScriptUiStage final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiStage);

  void ExecuteStageEffect(int32_t stage_value);

private:
  void Initiate() override final;
  void FinishEffect();

  opgs16::element::canvas::CText* m_stage_display = nullptr;
  ScriptObjectManagement* m_object_management = nullptr;

  opgs16::element::CTimerHandle m_effect_interval;

  int32_t m_stage = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H