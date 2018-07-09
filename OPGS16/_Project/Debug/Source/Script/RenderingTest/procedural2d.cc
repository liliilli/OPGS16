
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RenderingTest/procedural2d.h"

#include <Component/camera.h>
#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/Common/empty_object.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Object/RenderingTest/procedural2d_test.h"

namespace debug::script {

void Procedural2DRenderingTest::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;
  auto scene = GetPresentScene();

  m_selection = scene->GetGameObject("Canvas").get();

  auto subject = obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Procedural2D texture test");
  m_subject = subject;

  const auto description = obj.Instantiate<CText>("Desc", "");
  description->SetFontName("opSystem");
  description->SetFontSize(8);
  description->SetColor(opgs16::DColor::White);
  description->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  description->SetAlignment(IAlignable::Alignment::CENTER);
  description->SetWorldPosition({0.f, 48.f, 0.f});
  description->SetText("Look for procedural texture\non center.");
  m_description = description;

  auto camera_object = scene->Instantiate<object::EmptyObject>("Camera");
  auto cam = camera_object->AddComponent<opgs16::component::CCamera>(
      *camera_object,
      opgs16::component::CCamera::ViewType::PERSPECTIVE,
      opgs16::component::CCamera::CameraType::MAIN
  );
  cam->SetWorldPosition({128, 112, 100});
  cam->SetFov(90.f);
  cam->SetFar(250.f);
  cam->SetRear(1.f);

  scene->Instantiate<object::Procedural2DTestObject>("ProcedureObj");
}

void Procedural2DRenderingTest::Update(float delta_time) {

}

void Procedural2DRenderingTest::Destroy() {
  auto& obj = GetBindObject();
  obj.DestroyChild(*m_subject);
  obj.DestroyChild(*m_description);
}

} /// ::debug::script namespace