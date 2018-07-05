
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/float.h"

#include <limits>
#include <Phitos/Dbg/assert.h>

/// Random library
#include <Helper/Math/random.h>
#include <Manager/timer_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/Common/simplelog.h"

namespace debug::script {

void FloatTest::Initiate() {
  m_log = GetBindObject().Instantiate<object::SimpleLog>("Log", 5);
  m_log->SetOrigin(IOriginable::Origin::UP_LEFT);
  m_log->SetWorldPosition({16.f, -32.f, 0.f});

  OP16_TIMER_SET(m_timer, 20, true, this, &FloatTest::Tick);
}

void FloatTest::Update(float delta_time) { }

void FloatTest::Destroy() {
  GetBindObject().DestroyChild("Log");
}

void FloatTest::Tick() {
  if (m_count > m_test_count) {
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 500, true, this, &FloatTest::ExecuteSuccess);
    return;
  }

  const auto test = opgs16::random::RandomFloat();
  if (test <= std::numeric_limits<float>::max() &&
      test > std::numeric_limits<float>::lowest()) {
    // Success!

    if (m_count % m_set == 0) {
      m_log->PushLog("Float test Set.. " + std::to_string(m_count / m_set));
    }

    m_count += 1;
  }
  else {
    // Failure!
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 1'000, true, this, &FloatTest::ExecuteFailure);
  }
}

void FloatTest::ExecuteSuccess() {
  if (!m_is_color_changed)
    opgs16::manager::scene::GetPresentScene()->SetBackgroundColor(m_success);
  else
    opgs16::manager::scene::GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

void FloatTest::ExecuteFailure() {
  if (!m_is_color_changed)
    opgs16::manager::scene::GetPresentScene()->SetBackgroundColor(m_failure);
  else
    opgs16::manager::scene::GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

} /// ::debug::script namespace