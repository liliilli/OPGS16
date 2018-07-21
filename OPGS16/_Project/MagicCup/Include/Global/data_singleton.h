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

int32_t GetRankOf(const DScoreContainer& item);

///
/// @brief
///
bool IsIoBusy() noexcept;

bool SaveRankData(const DScoreContainer& item);

DScoreContainer& LoadRankData(int32_t index);

TScoreContainer& LoadRankDataContainer();

int32_t VerifyRankDataWithNew(const DScoreContainer& item);

void SaveAsRecentData(const DScoreContainer& item);

int32_t GetRecentDataRank() noexcept;

} /// ::magiccup namespace

#endif /// MAGICCUP_GLOBAL_DATA_SINGLETON_H