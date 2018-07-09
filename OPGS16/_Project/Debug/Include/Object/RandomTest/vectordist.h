#ifndef DEBUG_PROJECT_OBJECT_RANDOMTEST_VECTORDIST_H
#define DEBUG_PROJECT_OBJECT_RANDOMTEST_VECTORDIST_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace debug::object {

class VectorDistTestObject final : public opgs16::element::CObject {
public:
  VectorDistTestObject();
  void Render() override final;

private:
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_RANDOMTEST_VECTORDIST_H