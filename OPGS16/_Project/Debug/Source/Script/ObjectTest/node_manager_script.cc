
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Script/ObjectTest/node_manager_script.h"

#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/ObjectTest/node_cursor.h"
#include "../../../Include/Object/ObjectTest/node_object.h"

#include "../../../Include/Internal/keyword.h"

namespace debug::script {

void NodeManagerScript::Initiate() {
  auto scene = opgs16::manager::scene::GetPresentScene();

  auto _1 = scene->Instantiate<object::NodeObject>("Node");
  _1->SetWorldPosition({128.f, 180.f, 0});

  auto _2 = _1->Instantiate<object::NodeObject>("Node");
  _2->SetWorldPosition({-24.f, -24.f, 0});
  auto _6 = _1->Instantiate<object::NodeObject>("Node");
  _6->SetWorldPosition({ 24.f, -24.f, 0});

  auto _3 = _2->Instantiate<object::NodeObject>("Node");
  _3->SetWorldPosition({-24.f, -24.f, 0});

  auto _4 = _3->Instantiate<object::NodeObject>("Node");
  _4->SetWorldPosition({-24.f, -24.f, 0});
  auto _5 = _3->Instantiate<object::NodeObject>("Node");
  _5->SetWorldPosition({ 24.f, -24.f, 0});

  auto _7 = _6->Instantiate<object::NodeObject>("Node");
  _7->SetWorldPosition({-24.f, -24.f, 0});
  auto _8 = _6->Instantiate<object::NodeObject>("Node");
  _8->SetWorldPosition({ 24.f, -24.f, 0});

  auto _9 = _8->Instantiate<object::NodeObject>("Node");
  _9->SetWorldPosition({-24.f, -24.f, 0});
  auto _11 = _8->Instantiate<object::NodeObject>("Node");
  _11->SetWorldPosition({ 24.f, -24.f, 0});

  auto _10 = _9->Instantiate<object::NodeObject>("Node");
  _10->SetWorldPosition({-24.f, -24.f, 0});

  m_tree.reserve(11);
  m_tree.push_back(_1); m_tree.push_back(_2); m_tree.push_back(_3);
  m_tree.push_back(_4); m_tree.push_back(_5); m_tree.push_back(_6);
  m_tree.push_back(_7); m_tree.push_back(_8); m_tree.push_back(_9);
  m_tree.push_back(_10); m_tree.push_back(_11);

  m_cursor = scene->Instantiate<object::NodeCursorObject>("Cursor");
  UpdateCursorPosition(m_cursor_index);
}

void NodeManagerScript::Update(float delta_time) {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyReleased;
  using opgs16::manager::input::IsKeyPressed;

  if (m_is_pressed == true) {
    if (IsKeyReleased("Horizontal")) m_is_pressed = false;
    return;
  }

  if (const auto key_val = GetKeyValue("Horizontal"); key_val == 1.0f) { // Next
    if (m_cursor_index < m_tree.size() - 1) {
      ++m_cursor_index;
      UpdateCursorPosition(m_cursor_index);
    }
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) { // Previous
    if (m_cursor_index > 0) {
      --m_cursor_index;
      UpdateCursorPosition(m_cursor_index);
    }
    m_is_pressed = true;
  }

  if (IsKeyPressed(keyword::key_enter)) {
    switch (m_tree[m_cursor_index]->IsActive()) {
    case phitos::enums::EActivated::Disabled:
      m_tree[m_cursor_index]->SetActive(true);
      break;
    case phitos::enums::EActivated::Activated:
      m_tree[m_cursor_index]->SetActive(false);
      break;
    }
  }
}

void NodeManagerScript::UpdateCursorPosition(int32_t i) {
  m_cursor->SetWorldPosition(m_tree[i]->GetFinalPosition());
}

void NodeManagerScript::Destroy() {

}

} /// ::debug::script namespace