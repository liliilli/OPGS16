#ifndef MAGICCUP_TYPE_SCORE_CONTAINER_H
#define MAGICCUP_TYPE_SCORE_CONTAINER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <cstdint>
#include <string>

namespace magiccup {

struct DScoreContainer final {
  int32_t     score = 0;
  int32_t     stage = 0;
  std::string name = "AAA";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_TYPE_SCORE_CONTAINER_H