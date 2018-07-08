
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/float.h"

#include <limits>

/// Random library
#include <Helper/Math/random.h>
#include <Manager/input_manager.h>
#include <Manager/timer_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/sound_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/simplelog.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Script/RandomTest/manager.h"

namespace debug::script {

void FloatTest::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;
  m_object = GetPresentScene()->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Floating number test");
  m_subject = subject;

  m_log = obj.Instantiate<object::SimpleLog>("Log", 10);
  m_log->SetHeight(8);
  m_log->SetOrigin(IOriginable::Origin::UP_LEFT);
  m_log->SetWorldPosition({16.f, -32.f, 0.f});

  using opgs16::manager::sound::GenerateSound;
  GenerateSound("Success1");
  GenerateSound("Failure1");

  OP16_TIMER_SET(m_timer, 20, true, this, &FloatTest::Tick);
}

void FloatTest::Update(float delta_time) {
  if (opgs16::manager::input::IsKeyPressed("Back")) {
    Exit();
  }
}

void FloatTest::Destroy() {
  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);
  obj.DestroyChild(*m_log);
  OP16_TIMER_STOP(m_timer);
  OP16_TIMER_STOP(m_timer_return);
  opgs16::manager::sound::DestroySound("Success1");
  opgs16::manager::sound::DestroySound("Failure1");

  using opgs16::manager::scene::GetPresentScene;
  GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);
}

void FloatTest::Tick() {
  // If test is succeeded,
  if (m_count > m_test_count) {
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 500, true, this, &FloatTest::ExecuteSuccess);
    OP16_TIMER_SET(m_timer_return, 3'000, false, this, &FloatTest::Exit);
    opgs16::manager::sound::PlaySound("Success1");
    m_state = EState::Success;
    return;
  }

  // Test body
  const auto test = opgs16::random::RandomFloat();
  if (test <= std::numeric_limits<float>::max() &&
      test > std::numeric_limits<float>::lowest()) {
    if (m_count % m_set == 0) {
      m_log->PushLog("Float test Set.. " + std::to_string(m_count / m_set));
    }
    m_count += 1;
  }
  else {
    // Failure!
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 5'000, true, this, &FloatTest::ExecuteFailure);
    OP16_TIMER_SET(m_timer_return, 3'000, false, this, &FloatTest::Exit);
    opgs16::manager::sound::PlaySound("Failure1");
    m_state = EState::Failure;
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

void FloatTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ExecuteFloatTestToLobbyA();
  else PHITOS_UNEXPECTED_BRANCH();
}

} /// ::debug::script namespace