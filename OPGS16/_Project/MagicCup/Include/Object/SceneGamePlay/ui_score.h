#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_SCORE_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_SCORE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_object.h>

namespace magiccup {

class UiScore final : public opgs16::element::UiObject {
public:
  UiScore();

  inline static constexpr const char* s_object_name = "UiScore";

private:
  void LocalUpdate() override final;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_SCORE_H