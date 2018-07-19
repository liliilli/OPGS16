#ifndef MAGICCUP_OBJECT_CANVASITEM_BACKGROUND_H
#define MAGICCUP_OBJECT_CANVASITEM_BACKGROUND_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/image.h>

namespace magiccup {

class UiBackground final : public opgs16::element::canvas::CImage {
public:
  UiBackground(opgs16::element::canvas::CCanvas* canvas_object);

  inline static constexpr const char* s_object_name = "Background";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_CANVASITEM_BACKGROUND_H