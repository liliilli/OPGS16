
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Script/RandomTest/v3_range.h"

#include <Helper/Math/random.h>
#include <Manager/input_manager.h>
#include <Manager/object_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/orthogonal_camera.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Object/RandomTest/vector_range_dist_obj.h"

#include "../../../Include/Script/RandomTest/manager.h"
#include "../../../Include/Script/RandomTest/vector_dist_range.h"

namespace debug::script {

void Vector3RangeTest::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;

  auto scene = GetPresentScene();
  m_object = scene->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Vector2 Range distribution test");
  m_subject = subject;

  m_camera = scene->Instantiate<object::OrthogonalCameraObject>("Camera");

  m_xy_obj = scene->
      Instantiate<object::VectorDistRangeTestObject>("ProcedureObj")->
      GetComponent<script::VectorRangeDistribution>();
  m_xy_obj->GetBindObject().SetLocalPosition({-64.f, 0, 0});

  m_zy_obj = scene->
      Instantiate<object::VectorDistRangeTestObject>("ProcedureObj")->
      GetComponent<script::VectorRangeDistribution>();
  m_zy_obj->GetBindObject().SetLocalPosition({64.f, 0, 0});

  OP16_TIMER_SET(m_timer, 125, true, this, &Vector3RangeTest::Tick);
}

void Vector3RangeTest::Destroy() {
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);

  opgs16::manager::object::Destroy(m_xy_obj->GetBindObject());
  opgs16::manager::object::Destroy(m_zy_obj->GetBindObject());
  opgs16::manager::object::Destroy(*m_camera);

  OP16_TIMER_STOP(m_timer);
}

void Vector3RangeTest::Tick() {
  using opgs16::random::ERandomPolicy;
  const auto result = RandomVector3Range(ERandomPolicy::Uniform, 10.f, 15.f);
  m_xy_obj->InsertVector({result.x, result.y});
  m_zy_obj->InsertVector({result.z, result.y});
}

void Vector3RangeTest::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;

  if (IsKeyPressed("Back"))
    Exit();
}

void Vector3RangeTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ReturnFromVector3RangeTest();
  else PHITOS_UNEXPECTED_BRANCH();
}

} /// ::debug::script namespace