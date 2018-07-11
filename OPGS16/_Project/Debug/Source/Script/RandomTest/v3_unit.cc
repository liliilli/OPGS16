
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/v3_unit.h"

#include <Component/camera.h>
#include <Helper/math/random.h>
#include <Manager/input_manager.h>
#include <Manager/object_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/perspective_camera.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Object/RandomTest/vectordist.h"
#include "../../../Include/Script/RandomTest/manager.h"
#include "../../../Include/Script/RandomTest/vectordist.h"

namespace debug::script {

void Vector3UnitRandomTest::Initiate() {
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  auto scene = GetPresentScene();
  m_object = scene->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Vector3 Unit random dist test");
  m_subject = subject;

  auto xy = obj.Instantiate<CText>("Xy", "XY Axis");
  xy->SetFontName("opSystem");
  xy->SetFontSize(8);
  xy->SetAlignment(IAlignable::Alignment::CENTER);
  xy->SetOrigin(IOriginable::Origin::CENTER_CENTER);
  xy->SetWorldPosition({-64.f, -64.f, 0});
  m_axis_xy = xy;

  auto zy = obj.Instantiate<CText>("Zy", "ZY Axis");
  zy->SetFontName("opSystem");
  zy->SetFontSize(8);
  zy->SetAlignment(IAlignable::Alignment::CENTER);
  zy->SetOrigin(IOriginable::Origin::CENTER_CENTER);
  zy->SetWorldPosition({64.f, -64.f, 0});
  m_axis_zy = zy;

  m_camera = scene->Instantiate<object::PerspectiveCameraObject>("Camera");

  m_xy_obj = scene->Instantiate<object::VectorDistTestObject>("ProcObj")->GetComponent<VectorDistribution>();
  m_xy_obj->GetBindObject().SetLocalPosition({-64.f, 0, 0});

  m_zy_obj = scene->Instantiate<object::VectorDistTestObject>("ProcObj")->GetComponent<VectorDistribution>();
  m_zy_obj->GetBindObject().SetLocalPosition({64.f, 0, 0});

  OP16_TIMER_SET(m_timer, 250, true, this, &Vector3UnitRandomTest::Tick);
}

void Vector3UnitRandomTest::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;

  if (IsKeyPressed("Back"))
    Exit();
}

void Vector3UnitRandomTest::Tick() {
  const auto result = opgs16::random::RandomVector3Length(1.f);
  m_xy_obj->InsertVector({result.x, result.y});
  m_zy_obj->InsertVector({result.z, result.y});
}

void Vector3UnitRandomTest::Destroy() {
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);
  obj.DestroyChild(*m_axis_xy);
  obj.DestroyChild(*m_axis_zy);

  opgs16::manager::object::Destroy(m_xy_obj->GetBindObject());
  opgs16::manager::object::Destroy(m_zy_obj->GetBindObject());
  opgs16::manager::object::Destroy(*m_camera);

  OP16_TIMER_STOP(m_timer);
}

void Vector3UnitRandomTest::Exit() {
  const auto scr = m_object->GetComponent<script::RandomTestManager>();
  if (scr) scr->ReturnFromVector3Test();
  else PHITOS_UNEXPECTED_BRANCH();
}

} /// ::debug::script namespace