#ifndef PROJECT_DEBUG_OBJECT_SCENESELECT_MANAGER_H
#define PROJECT_DEBUG_OBJECT_SCENESELECT_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// ::opgs16::element::CObject
#include <Element/Canvas/canvas.h>

namespace debug::object {

class SelectManager final : public opgs16::element::canvas::CCanvas {
public:
  SelectManager();

  inline static constexpr const char* list_name = "List";
};

} /// ::debug::object

#endif /// PROJECT_DEBUG_OBJECT_SCENESELECT_MANAGER_H