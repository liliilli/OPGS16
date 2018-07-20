#ifndef MAGICCUP_OBJECT_SCENEMAIN_UI_BACKGROUND_PART_H
#define MAGICCUP_OBJECT_SCENEMAIN_UI_BACKGROUND_PART_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_default_object.h>

namespace magiccup {

class UiBackgroundPart final : public opgs16::element::CUiDefaultObject {
public:
  UiBackgroundPart();

  inline static constexpr const char* s_object_name = "Background";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEMAIN_UI_BACKGROUND_PART_H