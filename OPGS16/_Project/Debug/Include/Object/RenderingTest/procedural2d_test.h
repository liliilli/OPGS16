#ifndef DEBUG_PROJECT_OBJECT_RENDERINGTEST_PROCEDURAL2D_TEST_H
#define DEBUG_PROJECT_OBJECT_RENDERINGTEST_PROCEDURAL2D_TEST_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace debug::object {

class Procedural2DTestObject final : public opgs16::element::CObject {
public:
  Procedural2DTestObject();

  void Render() override final;

private:
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_RENDERINGTEST_PROCEDURAL2D_TEST_H