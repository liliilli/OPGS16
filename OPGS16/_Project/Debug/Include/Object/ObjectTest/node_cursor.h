#ifndef DEBUG_PROJECT_OBJECT_OBJECTTEST_NODE_CURSOR_H
#define DEBUG_PROJECT_OBJECT_OBJECTTEST_NODE_CURSOR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace debug::object {

class NodeCursorObject final : public opgs16::element::CObject {
public:
  NodeCursorObject();

private:
  void Render() override final;

  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_OBJECTTEST_NODE_CURSOR_H