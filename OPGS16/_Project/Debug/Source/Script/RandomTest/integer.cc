
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/integer.h"

#include <limits>

#include <Element/Canvas/text.h>
/// Random library
#include <Helper/Math/random.h>
#include <Manager/input_manager.h>
#include <Manager/timer_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/sound_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/simplelog.h"
#include "../../../Include/Script/RandomTest/manager.h"

namespace debug::script {

void IntegerTest::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;
  m_object = GetPresentScene()->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<CText>("Subject", "Integer number test");
  subject->SetFontName("Hangul");
  subject->SetOrigin(IOriginable::Origin::UP_CENTER);
  subject->SetAlignment(IAlignable::Alignment::CENTER);
  subject->SetFontSize(16);
  subject->SetWorldPosition({0.f, -24.f, 0.f});
  m_subject = subject;

  m_log = obj.Instantiate<object::SimpleLog>("Log", 10);
  m_log->SetHeight(8);
  m_log->SetOrigin(IOriginable::Origin::UP_LEFT);
  m_log->SetWorldPosition({16.f, -32.f, 0.f});

  using opgs16::manager::sound::GenerateSound;
  GenerateSound("Success1");

  OP16_TIMER_SET(m_timer, 20, true, this, &IntegerTest::Tick);
}

void IntegerTest::Destroy() {
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

void IntegerTest::Tick() {
  // If test is succeeded,
  if (m_count > m_test_count) {
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 500, true, this, &IntegerTest::ExecuteSuccess);
    OP16_TIMER_SET(m_timer_return, 3'000, false, this, &IntegerTest::Exit);
    opgs16::manager::sound::PlaySound("Success1");
    m_state = EState::Success;
    return;
  }

  // Test body
  const auto test = opgs16::random::RandomInteger();
  if (test <= std::numeric_limits<int32_t>::max() &&
      test > std::numeric_limits<int32_t>::lowest()) {
    if (m_count % m_set == 0) {
      m_log->PushLog("Integer test Set.. " + std::to_string(m_count / m_set));
    }
    m_count += 1;
  }
  else {
    // Failure!
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer, 5'000, true, this, &IntegerTest::ExecuteFailure);
    OP16_TIMER_SET(m_timer_return, 3'000, false, this, &IntegerTest::Exit);
    opgs16::manager::sound::PlaySound("Failure1");
    m_state = EState::Failure;
  }
}

void IntegerTest::ExecuteSuccess() {
  using opgs16::manager::scene::GetPresentScene;
  if (!m_is_color_changed)
    GetPresentScene()->SetBackgroundColor(m_success);
  else
    GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

void IntegerTest::ExecuteFailure() {
  using opgs16::manager::scene::GetPresentScene;
  if (!m_is_color_changed)
    GetPresentScene()->SetBackgroundColor(m_failure);
  else
    GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

void IntegerTest::Update(float delta_time) {
  if (opgs16::manager::input::IsKeyPressed("Back")) {
    Exit();
  }
}

void IntegerTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ExecuteIntegerTestToLobbyA();
  else PHITOS_UNEXPECTED_BRANCH();
}


} /// ::debug::script