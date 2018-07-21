#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_DATA_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_DATA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {
class ScriptUiScore;
class ScriptUiLife;
class ScriptUiTimelimit;
class ScriptUiStage;
class ScriptObjectManagement;
} /// Forward declaration

namespace magiccup {

class ScriptDataContainer final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptDataContainer);

  int32_t GetScore() const noexcept;

  int32_t GetLife() const noexcept;

  int32_t GetStage() const noexcept;

  void SetLife(int32_t value) noexcept;

  void SetScore(int32_t value) noexcept;

  void RepeatStage() noexcept;

  void IncrementStage() noexcept;

  int32_t GetShakingCount() noexcept;

  int32_t GetShakingInterval() noexcept;

  int32_t GetTimerLimitValue() noexcept;

  void SetTimerValue(int32_t value) noexcept;

  void StoreData() const;

private:
  void Initiate() override final;

  int32_t CalculateScore();

  int32_t m_score = 0;
  int32_t m_life  = 2;
  int32_t m_stage = 0;
  int32_t m_time_limit = 10'000;
  int32_t m_initial_time_limit = 10'000;
  int32_t m_shake_interval = 1'500;
  int32_t m_shaking = 2;

  const int32_t m_shake_intv_lim = 300;
  const int32_t m_shake_intv_dec = 100;
  const int32_t m_shake_intv_inc = 100;
  const int32_t m_life_lim = 5;
  const int32_t m_time_limt = 2'500;
  const int32_t m_time_intv_dec = 500;
  const int32_t m_extended = 30'000;

  const int32_t m_4th_cup_score = 20'000;
  const int32_t m_5th_cup_score = 60'000;

  ScriptUiScore* m_script_score = nullptr;
  ScriptUiLife* m_script_life = nullptr;
  ScriptUiTimelimit* m_script_timelimit = nullptr;
  ScriptUiStage* m_script_stage = nullptr;
  ScriptObjectManagement* m_object_management = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_DATA_H