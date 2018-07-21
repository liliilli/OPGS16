#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_EFFECT_BOOM_OBJECT_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_EFFECT_BOOM_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class EffectBoomObject final : public opgs16::element::CObject {
public:
  EffectBoomObject();

private:
  void Render() override final;

  opgs16::component::CSprite2DRenderer* m_renderer = nullptr;
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;

  opgs16::component::CAnimator* m_animator = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_EFFECT_BOOM_OBJECT_H