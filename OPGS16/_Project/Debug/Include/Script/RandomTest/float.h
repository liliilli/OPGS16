#pragma once

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace debug::object {
class SimpleLog;
}

namespace debug::script {

class FloatTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(FloatTest);

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;

  void Destroy() override final;

  object::SimpleLog* m_log = nullptr;

  enum class EState {
    Processing,
    Success,
    Failure
  };

  EState m_state = EState::Processing;

  int32_t m_test_count = 10'000;
  int32_t m_set = 100;
};

} /// ::debug::script namespace