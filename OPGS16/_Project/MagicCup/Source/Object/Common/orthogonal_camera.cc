
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/orthogonal_camera.h"

#include <Component/camera.h>

namespace magiccup {

OrthogonalCameraObject::OrthogonalCameraObject() {
  using opgs16::component::CCamera;
  using EViewType = CCamera::ViewType;
  using ECameraType = CCamera::CameraType;
  AddComponent<CCamera>(*this, EViewType::ORTHO, ECameraType::MAIN);
}

} /// ::debug::object namespace