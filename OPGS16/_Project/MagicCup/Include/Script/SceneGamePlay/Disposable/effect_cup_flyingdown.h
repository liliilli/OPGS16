#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_CUP_FLYINGDOWN_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_CUP_FLYINGDOWN_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class EffectCupFlyingDown final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(EffectCupFlyingDown);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;


};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_DISPOSABLE_EFFECT_CUP_FLYINGDOWN_H