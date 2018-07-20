
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Scoreboard/ui_rank_text.h"

#include "../../../Include/Internal/general_keyword.h"

namespace {

std::string CreateScoreText(int32_t score_value) {
  std::string result;
  result.reserve(6);

  std::string ad = std::to_string(score_value);
  int32_t df = 6 - static_cast<int32_t>(ad.size());

  while (df > 0) {
    result.append("0");
    --df;
  }

  return result.append(ad);
}

std::string CreateStageText(int32_t stage_value) {
  std::string result;
  result.reserve(3);

  std::string ad = std::to_string(stage_value);
  int32_t df = 3 - static_cast<int32_t>(ad.size());

  while (df > 0) {
    result.append("0");
    --df;
  }

  return result.append(ad);;
}

std::string CreateRankText(int32_t rank_value) {
  if (rank_value >= 10) {
    return std::to_string(rank_value);
  }

  std::string result = "0";
  result += rank_value + '0';
  return result;
}

} /// unnamed namespace

namespace magiccup {

UiRankText::UiRankText() : CText{""} {
  SetFontSize(8);
  SetFontName(keyword::fon_default);
  SetOrigin(Origin::UP_CENTER);
  SetAlignment(Alignment::CENTER);
  SetColor(opgs16::DColor::White);
  SetRenderingLayer(3);
  SetLocalPosition({0, -4, 0});
}

void UiRankText::SetTextContainer(
    int32_t rank,
    const data::TScoreContainer::value_type& container) {
  if (rank <= 1) {
    SetColor(opgs16::DColor::Gold);
  }
  else if (rank <= 2) {
    SetColor(opgs16::DColor::Green);
  }
  else if (rank <= 3) {
    SetColor(opgs16::DColor::Aqua);
  }

  std::string result;
  result.reserve(25);
  result.append(CreateRankText(rank));
  result.append("  ");
  result.append(CreateScoreText(container.score));
  result.append("        ");
  result.append(CreateStageText(container.stage));
  result.append("    ");
  result.append(container.name);
  SetText(result);
}

} /// ::magiccup namespace