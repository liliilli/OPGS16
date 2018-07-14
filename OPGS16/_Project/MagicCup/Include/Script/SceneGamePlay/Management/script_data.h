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
} /// Forward declaration

namespace magiccup {

class ScriptDataContainer final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptDataContainer);

  int32_t GetScore() const noexcept;

  void SetScore(int32_t value) noexcept;

  int32_t GetLife() const noexcept;

  void SetLife(int32_t value) noexcept;

  void IncrementStage() noexcept;

  int32_t GetShakingCount() noexcept {
    return m_shaking;
  }

private:
  void Initiate() override final;

  int32_t m_score = 0;
  int32_t m_life = 3;
  int32_t m_stage = 0;
  int32_t m_time_limit = 100'000;
  int32_t m_shaking = 2;

  ScriptUiScore* m_script_score = nullptr;
  ScriptUiLife* m_script_life = nullptr;
  ScriptUiTimelimit* m_script_timelimit = nullptr;
  ScriptUiStage* m_script_stage = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_DATA_H