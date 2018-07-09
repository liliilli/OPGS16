
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/Test/object_lifecycle.h"

#include "../../../Include/Object/Common/empty_canvas.h"
#include "../../../Include/Object/Common/empty_object.h"
#include "../../../Include/Object/LifecycleTest/lifecycle_manager.h"

#include <Element/Canvas/text.h>

/// ::opgs16::component::CCamera
#include <Component/camera.h>
#include <Component/font2d_renderer.h>

namespace debug::scene {

void ObjectLifeCycle::Initiate() {
  Instantiate<object::LifecycleManager>("Manager");
  auto camera_object = Instantiate<object::EmptyObject>("Camera");
  auto cam = camera_object->AddComponent<opgs16::component::CCamera>(
      *camera_object,
      opgs16::component::CCamera::ViewType::PERSPECTIVE,
      opgs16::component::CCamera::CameraType::MAIN
  );
  cam->SetWorldPosition({128, 112, 100});
  cam->SetFov(90.f);
  cam->SetFar(250.f);
  cam->SetRear(1.f);

  auto canvas = Instantiate<object::EmptyCanvas>("Canvas");
  auto text = canvas->Instantiate<opgs16::element::canvas::CText>("Object", "Hello world!");
  text->GetComponent<opgs16::component::CFont2DRenderer>()->SetRenderLayer("Interface");
  text->SetFontName("Solomon");
  text->SetFontSize(16);
  text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
}

} /// ::debug::scene
