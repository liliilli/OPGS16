
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Source/Scene/debug_aabb.cc
///
/// @brief
/// Text debug scene.
///
/// @log
/// 2018-05-13 Create file.
///

/// Header file
#include "../../Include/Scene/debug_aabb.h"

/// ::debug::object::RotatableObstacle
#include "../../Include/Object/rotatable_obstacle.h"

#include "../../Include/Object/empty_object.h"
/// ::opgs16::components::CCamera
#include <Component/camera.h>

namespace debug::scene {

void DebugAABB::Initiate() {
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

  Instantiate<object::RotatableObject>("Obstacle");
}


} /// ::debug::scene