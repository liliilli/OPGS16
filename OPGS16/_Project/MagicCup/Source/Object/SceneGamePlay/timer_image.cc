
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/timer_image.h"

#include <Component/sprite2d_renderer.h>
#include <Helper/Type/color.h>

namespace magiccup {

TimerImage::TimerImage(const opgs16::element::canvas::CCanvas* canvas_reference) :
    CImage{"opSystem", canvas_reference} {
  using opgs16::component::CSprite2DRenderer;
  using opgs16::DColor;

  SetTextureFragmentIndex(0);
  SetLocalPosition({0.f, 4.f, 0.f});
  SetRenderingLayer(3);

  auto renderer = GetComponent<CSprite2DRenderer>();
  renderer->SetShader("plainColor");
  m_renderer = renderer;
  SetColor(DColor::Aqua);
}

void TimerImage::SetColor(const opgs16::DColor& color) {
  m_renderer->GetWrapper().SetUniformVec3(
      "uColor",
      static_cast<glm::vec3>(static_cast<opgs16::DVector3>(color))
  );
}

} /// ::magiccup namespace