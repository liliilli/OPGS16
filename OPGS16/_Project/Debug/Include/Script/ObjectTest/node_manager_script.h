#ifndef DEBUG_PROJECT_SCRIPT_OBJECTTEST_NODE_MANAGER_SCRIPT_H
#define DEBUG_PROJECT_SCRIPT_OBJECTTEST_NODE_MANAGER_SCRIPT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <vector>

#include <Component/script_frame.h>

namespace debug::script {

class NodeManagerScript final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(NodeManagerScript);

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override final;

  void UpdateCursorPosition(int32_t i);

  std::vector<opgs16::element::CObject*> m_tree;
  opgs16::element::CObject* m_cursor = nullptr;

  int32_t m_cursor_index = 0;
  bool m_is_pressed = false;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_OBJECTTEST_NODE_MANAGER_SCRIPT_H