#ifndef MAGICCUP_OBJECT_COMMON_FADE_OUT_H
#define MAGICCUP_OBJECT_COMMON_FADE_OUT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include <Element/ui_default_object.h>

namespace opgs16::component {
class CProcedural2DRenderer;
} /// ::opgs16::component namespace

namespace magiccup {

class UiFadeOut final : public opgs16::element::CUiDefaultObject {
public:
  UiFadeOut(int32_t time,
      std::function<void(void)> callback,
      opgs16::element::canvas::CCanvas* canvas_ref,
      float intensity = 1.0f);

  inline static constexpr const char* s_object_name = "FadeOut";

private:
  void Render() override final;

  opgs16::component::CProcedural2DRenderer* m_renderer = nullptr;
  opgs16::element::canvas::CCanvas* m_canvas_ref = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_COMMON_FADE_OUT_H