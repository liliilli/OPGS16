
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../Include/Global/data_singleton.h"

#include <array>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <mutex>

#include <Headers/import_logger.h>
#include <Phitos/Dbg/assert.h>

#include "../../Include/Type/score_container.h"

namespace {

using TScore = int32_t;
using TStage = int32_t;
using TDefaultType = std::tuple<TScore, TStage, std::string_view>;

constexpr int32_t s_game_version = 20180719;

constexpr std::string_view s_file_name = "./setting.sav";
constexpr std::array<TDefaultType, 10> s_default_score = {
  TDefaultType{10000, 10, "AAA"},
  TDefaultType{9000,  9,  "BBB"},
  TDefaultType{8000,  8,  "CCC"},
  TDefaultType{7000,  7,  "DDD"},
  TDefaultType{6000,  6,  "EEE"},
  TDefaultType{5000,  5,  "FFF"},
  TDefaultType{4000,  4,  "GGG"},
  TDefaultType{3000,  3,  "HHH"},
  TDefaultType{2000,  2,  "III"},
  TDefaultType{1000,  1,  "JJJ"}
};

std::atomic<bool> s_io_busy = false;
std::mutex s_io_mutex;

bool s_is_first_time = false;
std::array<magiccup::DScoreContainer, 10> s_scoreboard;

void InitializeScoreboard() noexcept {
  for (int32_t i = 0; i < 10; ++i) {
    s_scoreboard[i].score = std::get<0>(s_default_score[i]);
    s_scoreboard[i].stage = std::get<1>(s_default_score[i]);
    s_scoreboard[i].name  = std::get<2>(s_default_score[i]);
  }
}

template <typename TType>
void WriteToken(std::ofstream& file_stream, const TType& value) {
  if constexpr (std::is_arithmetic_v<TType>) {
    file_stream.write(reinterpret_cast<const char*>(&value), sizeof(TType));
  }
  else if constexpr (std::is_same_v<std::string, std::decay_t<TType>>) {
    const char* string = value.c_str();
    const int32_t size = strlen(string);

    file_stream.write(reinterpret_cast<const char*>(&size), sizeof(int32_t));
    file_stream.write(string, size);
  }
}

template <typename TType,
          typename = std::enable_if_t<!std::is_const_v<TType>>
>
void ReadToken(std::ifstream& file_stream, TType& instance) {
  if constexpr (std::is_arithmetic_v<TType>) {
    char string[sizeof(TType)] = { '\0', };
    file_stream.read(string, sizeof(TType));
    instance = *(reinterpret_cast<TType*>(string));
  }
  else if constexpr (std::is_same_v<std::string, std::decay_t<TType>>) {
    char size_string[sizeof int32_t] = { '\0', };
    file_stream.read(size_string, sizeof(int32_t));
    const int32_t size = *(reinterpret_cast<int32_t*>(size_string));

    char* string = new char[size + 1];
    file_stream.read(string, size);
    instance = std::string(string, size);
    delete[] string;
  }
}

} /// unnamed namespace (data)

namespace magiccup::data {

bool IsSaveFileExist() {
  return std::filesystem::exists(s_file_name);
}

bool IsIoBusy() noexcept {
  return s_io_busy;
}

bool LoadSaveFile() {
  s_io_busy = true;
  std::lock_guard<std::mutex> safe_lock_guard{s_io_mutex};

  if (!IsSaveFileExist()) {
    PUSH_LOG_CRITICAL_EXT(
        "File is not exist. Failed to load file. [Path : {}]",
        std::filesystem::absolute(s_file_name).string());
    return false;
  }

  std::ifstream file_stream { s_file_name,
                              std::ifstream::binary | std::ifstream::in };
  if (!file_stream.good()) {
    return false;
  }

  ReadToken(file_stream, s_is_first_time);
  for (auto& element : s_scoreboard) {
    ReadToken(file_stream, element.score);
    ReadToken(file_stream, element.stage);
    ReadToken(file_stream, element.name);
  }

  file_stream.close();
  s_io_busy = false;
  return true;
}

bool SaveSaveFile() {
  s_io_busy = true;
  std::lock_guard<std::mutex> safe_lock_guard{s_io_mutex};

  if (!IsSaveFileExist()) {
    InitializeScoreboard();
  }

  std::ofstream file_stream { s_file_name,
                              std::ofstream::binary | std::ofstream::out };
  if (!file_stream.good()) {
    return false;
  }

  WriteToken(file_stream, s_is_first_time);
  for (const auto& element : s_scoreboard) {
    WriteToken(file_stream, element.score);
    WriteToken(file_stream, element.stage);
    WriteToken(file_stream, element.name);
  };

  file_stream.close();
  s_io_busy = false;
  return true;
}

TScoreContainer& LoadRankDataContainer() {
  return s_scoreboard;
}

bool SaveRankData(const DScoreContainer& item) {
  const auto satisfying_it =
      std::find_if(s_scoreboard.begin(), s_scoreboard.end(),
      [&item](const DScoreContainer& elem) {
        return item.score >= elem.score;
      }
  );

  if (satisfying_it == s_scoreboard.end())
    return false;

  const int32_t rev_index = std::distance(s_scoreboard.begin(), satisfying_it);
  const int32_t last_index = s_scoreboard.size() - 1;

  for (int32_t i = last_index; i > rev_index; --i) {
    if (i == last_index)
      continue;

    s_scoreboard[i] = s_scoreboard[i + 1];
  }

  s_scoreboard[rev_index] = item;
  return true;
}

const DScoreContainer& LoadRankData(int32_t index) {
  PHITOS_ASSERT(index < s_scoreboard.size(), "Index must be less than.");
  PHITOS_ASSERT(index >= 0, "Index must be bigger than -1.");

  return s_scoreboard[index];
}

} /// ::magiccup::data namespace