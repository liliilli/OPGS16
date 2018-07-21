#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_CHAR_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_CHAR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/text.h>

namespace magiccup {

class UiCharRouletteObject final : public opgs16::element::canvas::CText {
public:
  UiCharRouletteObject();

  inline static constexpr const char* s_gameobject_name = "Char";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_CHAR_H