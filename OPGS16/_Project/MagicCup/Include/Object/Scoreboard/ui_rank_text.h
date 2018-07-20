#ifndef MAGICCUP_OBJECT_SCOREBOARD_UI_RANK_TEXT_H
#define MAGICCUP_OBJECT_SCOREBOARD_UI_RANK_TEXT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/text.h>

#include "../../Global/data_singleton.h"

namespace magiccup {

class UiRankText final : public opgs16::element::canvas::CText {
public:
  UiRankText();

  void SetTextContainer(int32_t rank, const data::TScoreContainer::value_type& container);
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCOREBOARD_UI_RANK_TEXT_H