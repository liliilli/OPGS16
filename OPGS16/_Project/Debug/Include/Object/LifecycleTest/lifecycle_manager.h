#ifndef DEBUG_OBJECT_LIFECYCLE_MANAGER_H
#define DEBUG_OBJECT_LIFECYCLE_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Object/lifecycle_manager.h
///

/// ::opgs16::element::CObject
#include <Element/object.h>

namespace debug::object {

class LifecycleManager final : public opgs16::element::CObject {
public:
  LifecycleManager();
};

} /// ::debug::object

#endif /// DEBUG_OBJECT_LIFECYCLE_MANAGER_H