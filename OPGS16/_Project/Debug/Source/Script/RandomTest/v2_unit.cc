
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/v2_unit.h"

#include <Component/camera.h>
#include <Helper/math/random.h>
#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/sound_manager.h>
#include <Manager/timer_manager.h>

#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/empty_object.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Object/RandomTest/vectordist.h"
#include "../../../Include/Script/RandomTest/manager.h"
#include "../../../Include/Script/RandomTest/vectordist.h"

namespace debug::script {

void Vector2UnitRandomTest::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;

  auto scene = GetPresentScene();
  m_object = scene->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Vector2 Unit random dist test");
  m_subject = subject;

  auto camera_object = scene->Instantiate<object::EmptyObject>("Camera");
  auto cam = camera_object->AddComponent<opgs16::component::CCamera>(
      *camera_object,
      opgs16::component::CCamera::ViewType::PERSPECTIVE,
      opgs16::component::CCamera::CameraType::MAIN
  );
  cam->SetWorldPosition({128, 112, 100});
  cam->SetFov(90.f);
  cam->SetRear(90.f);
  cam->SetFar(110.f);

  m_dist_obj = scene->
      Instantiate<object::VectorDistTestObject>("ProcedureObj")->
      GetComponent<script::VectorDistribution>();

  OP16_TIMER_SET(m_timer, 500, true, this, &Vector2UnitRandomTest::Tick);
}

void Vector2UnitRandomTest::Destroy() {
  using opgs16::manager::sound::DestroySound;
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);

  OP16_TIMER_STOP(m_timer);
  OP16_TIMER_STOP(m_timer_return);

  GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);
}

void Vector2UnitRandomTest::Tick() {
  const auto result = opgs16::random::RandomVector2Length(1.f);
  m_dist_obj->InsertVector(result);
}

void Vector2UnitRandomTest::ExecuteSuccess() {
  using opgs16::manager::scene::GetPresentScene;
  if (!m_is_color_changed)
    GetPresentScene()->SetBackgroundColor(m_success);
  else
    GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

void Vector2UnitRandomTest::ExecuteFailure() {
  using opgs16::manager::scene::GetPresentScene;
  if (!m_is_color_changed)
    GetPresentScene()->SetBackgroundColor(m_failure);
  else
    GetPresentScene()->SetBackgroundColor(opgs16::DColor::Black);

  m_is_color_changed = !m_is_color_changed;
}

void Vector2UnitRandomTest::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;

  if (IsKeyPressed("Back"))
    Exit();
}

void Vector2UnitRandomTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ExecuteIntegerTestToLobbyA();
  else PHITOS_UNEXPECTED_BRANCH();
}

} /// ::debug::script namespace