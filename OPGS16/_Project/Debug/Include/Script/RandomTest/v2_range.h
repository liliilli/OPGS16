#ifndef DEBUG_PROJECT_SCRIPT_RANDOMTEST_V2_RANGE_H
#define DEBUG_PROJECT_SCRIPT_RANDOMTEST_V2_RANGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace debug::script {
class VectorRangeDistribution;
}

namespace debug::script {

class Vector2RangeTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(Vector2RangeTest);

private:
  opgs16::element::CObject* m_subject = nullptr;
  opgs16::element::CObject* m_object = nullptr;
  opgs16::element::CObject* m_camera = nullptr;

  script::VectorRangeDistribution* m_dist_obj = nullptr;

  opgs16::element::CTimerHandle m_timer;

  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override final;

  void Tick();
  void Exit();
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_RANDOMTEST_V2_RANGE_H