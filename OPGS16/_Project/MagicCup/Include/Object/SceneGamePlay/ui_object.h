#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_CANVAS_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_CANVAS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_object.h>

namespace magiccup {

class GamePlayUi final : public opgs16::element::UiObject {
public:
  GamePlayUi();

  inline static constexpr const char* s_obj_name = "GameUi";

private:
  void LocalUpdate() override final;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_UI_CANVAS_H