
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Scene/object_lifecycle.cc
///

/// Header file
#include "../../Include/Scene/object_lifecycle.h"

#include "../../Include/Object/empty_canvas.h"
#include "../../Include/Object/empty_object.h"
#include "../../Include/Object/lifecycle_manager.h"

#include <Element/Canvas/text.h>

/// ::opgs16::component::CCamera
#include <Component/camera.h>
#include "Component/font2d_renderer.h"

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
  text->SetFontSize(24);
  text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
}

} /// ::debug::scene
