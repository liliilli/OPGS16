#pragma once

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>
#include <Helper/Type/color.h>

namespace debug::object {
class SimpleLog;
}

namespace debug::script {

class FloatTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(FloatTest);

private:
  object::SimpleLog* m_log = nullptr;

  enum class EState { Processing, Success, Failure };
  EState m_state = EState::Processing;

  opgs16::element::CTimerHandle m_timer;

  int32_t m_test_count = 100;
  int32_t m_set = 20;
  int32_t m_count = 1;

  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override final;

  opgs16::DColor m_success = opgs16::DColor::Blue;
  opgs16::DColor m_failure = opgs16::DColor::Red;

  void Tick();
  void ExecuteSuccess();
  void ExecuteFailure();
};

} /// ::debug::script namespace