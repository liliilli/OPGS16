
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Object/Common/perspective_camera.h"

#include <Component/camera.h>

namespace debug::object {

PerspectiveCameraObject::PerspectiveCameraObject() {
  auto cam = AddComponent<opgs16::component::CCamera>(*this,
      opgs16::component::CCamera::ViewType::PERSPECTIVE,
      opgs16::component::CCamera::CameraType::MAIN
  );
  cam->SetWorldPosition({128, 112, 100});
  cam->SetFov(90.f);
  cam->SetRear(90.f);
  cam->SetFar(110.f);
}

} /// ::debug::object namespace