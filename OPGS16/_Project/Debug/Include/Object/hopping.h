#ifndef DEBUG_OBJECT_HOPPING_H
#define DEBUG_OBJECT_HOPPING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Object/hopping.h
///

/// ::opgs16::element::CObject
#include <Element/object.h>

/// Forward declarations
#include <opgs16fwd.h>

namespace debug::object {

class Hopping final : public opgs16::element::CObject {
public:
  Hopping();

private:
  void Render() override final;

  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object

#endif /// DEBUG_OBJECT_HOPPING_H
