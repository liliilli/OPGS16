#ifndef MAGICCUP_GLOBAL_DATA_SINGLETON_H
#define MAGICCUP_GLOBAL_DATA_SINGLETON_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../Type/score_container.h"

namespace magiccup::data {

using TScoreContainer = std::array<magiccup::DScoreContainer, 10>;

bool IsSaveFileExist();

///
/// @brief If save file is not found, create new default save file.
///
bool LoadSaveFile();

bool SaveSaveFile();

///
/// @brief
///
bool IsIoBusy() noexcept;

bool SaveRankData(const DScoreContainer& item);

const DScoreContainer& LoadRankData(int32_t index);

TScoreContainer& LoadRankDataContainer();

} /// ::magiccup namespace

#endif /// MAGICCUP_GLOBAL_DATA_SINGLETON_H