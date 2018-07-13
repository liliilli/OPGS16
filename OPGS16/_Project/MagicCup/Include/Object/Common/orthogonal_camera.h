#ifndef DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H
#define DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class OrthogonalCameraObject final : public opgs16::element::CObject {
public:
  OrthogonalCameraObject();

  inline static constexpr const char* s_object_name = "OrthoCam";
};

} /// ::magiccup namespace

#endif /// DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H