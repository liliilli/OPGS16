#ifndef DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H
#define DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace debug::object {

class OrthogonalCameraObject final : public opgs16::element::CObject {
public:
  OrthogonalCameraObject();
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_COMMON_ORTHOGONAL_CAMERA_H