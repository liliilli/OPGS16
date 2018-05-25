
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

#include "../../Include/Object/empty_object.h"
#include "../../Include/Object/lifecycle_manager.h"

/// ::opgs16::component::CCamera
#include <Component/camera.h>

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
}

} /// ::debug::scene
