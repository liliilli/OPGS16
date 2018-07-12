#ifndef TEST_PROJECT_OBJECT_RANDOMTEST_VECTOR_RANGE_DIST_OBJ_H
#define TEST_PROJECT_OBJECT_RANDOMTEST_VECTOR_RANGE_DIST_OBJ_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace debug::object {

class VectorDistRangeTestObject final : public opgs16::element::CObject {
public:
  VectorDistRangeTestObject();
  void Render() override final;

private:
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object namespace

#endif /// TEST_PROJECT_OBJECT_RANDOMTEST_VECTOR_RANGE_DIST_OBJ_H