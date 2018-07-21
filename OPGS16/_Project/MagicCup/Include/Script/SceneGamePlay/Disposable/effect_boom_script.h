#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BOOM_SCRIPT_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BOOM_SCRIPT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptEffectBoom final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptEffectBoom);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  opgs16::component::CAnimator* m_animator = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_BOOM_SCRIPT_H