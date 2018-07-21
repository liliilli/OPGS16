#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BACK_FAILURE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BACK_FAILURE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

//!
//! Forward declaration
//!

namespace magiccup {
class ScriptUiBackground;
}

//!
//! Implementation
//!

namespace magiccup {

class EffectBackgroundFailure final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(EffectBackgroundFailure);

private:
  void Initiate() override final;

  void CbTickEffect();

  ScriptUiBackground* m_script = nullptr;

  opgs16::element::CTimerHandle m_timer_handle;

  int32_t m_count     = 32;
  bool m_is_turned_on = false;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BACK_FAILURE_H
