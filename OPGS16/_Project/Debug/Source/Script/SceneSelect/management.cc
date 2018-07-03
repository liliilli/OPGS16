
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneSelect/management.h"

#include <Element/object.h>
#include <Manager/input_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/SceneSelect/choice_list.h"
#include "../../../Include/Object/SceneSelect/select_manager.h"

namespace debug::script {

void Management::Initiate() {
  m_list = static_cast<object::ChoiceList*>(
      GetBindObject().GetChild(object::SelectManager::list_name)
  );

  if (m_list == nullptr) {
    PHITOS_UNEXPECTED_BRANCH();
  }
}

void Management::Start() {

}

void Management::Update(float delta_time) {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyReleased;
  const auto key_val = GetKeyValue("Vertical");

  if (m_is_pressed == true) {
    if (IsKeyReleased("Vertical")) m_is_pressed = false;
    return;
  }

  if (key_val == 1.0f) {
    m_list->MoveCursor(object::EDirection::Up);
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    m_list->MoveCursor(object::EDirection::Down);
    m_is_pressed = true;
  }


}

} /// ::debug::script namespace