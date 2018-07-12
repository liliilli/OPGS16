
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
#include <Manager/object_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/orthogonal_camera.h"
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

  m_camera = scene->Instantiate<object::OrthogonalCameraObject>("Camera");

  m_dist_obj = scene->
      Instantiate<object::VectorDistTestObject>("ProcedureObj")->
      GetComponent<script::VectorDistribution>();

  OP16_TIMER_SET(m_timer, 250, true, this, &Vector2UnitRandomTest::Tick);
}

void Vector2UnitRandomTest::Destroy() {
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);
  opgs16::manager::object::Destroy(m_dist_obj->GetBindObject());
  opgs16::manager::object::Destroy(*m_camera);

  OP16_TIMER_STOP(m_timer);
}

void Vector2UnitRandomTest::Tick() {
  const auto result = opgs16::random::RandomVector2Length(1.f);
  m_dist_obj->InsertVector(result);
}

void Vector2UnitRandomTest::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;

  if (IsKeyPressed("Back"))
    Exit();
}

void Vector2UnitRandomTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ReturnFromVector2Test();
  else PHITOS_UNEXPECTED_BRANCH();
}

} /// ::debug::script namespace