#ifndef MAGICCUP_OBJECT_SCENEMAIN_UI_TITLE_PART_H
#define MAGICCUP_OBJECT_SCENEMAIN_UI_TITLE_PART_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_default_object.h>

namespace magiccup {

class UiTitlePart final : public opgs16::element::CUiDefaultObject {
public:
  UiTitlePart();

  inline static constexpr const char* s_object_name = "TitlePart";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEMAIN_UI_TITLE_PART_H