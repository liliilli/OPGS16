#ifndef DEBUG_OBJECT_ROTATABLE_OBSTACLE_H
#define DEBUG_OBJECT_ROTATABLE_OBSTACLE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Object/rotatable_obstacle.h
///

/// ::opgs16::element::CObject
#include <Element/object.h>
/// Forward declarations
#include <opgs16fwd.h>

namespace debug::object {

class RotatableObject final : public opgs16::element::CObject {
public:
  RotatableObject();

private:
  void Render() override;

  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object namespace

#endif /// DEBUG_OBJECT_ROTATABLE_OBSTACLE_H