
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/total_management.h"

#include "../../../Include/Script/SceneGamePlay/Management/script_key_input.h"
#include "../../../Include/Script/SceneGamePlay/Management/script_data.h"
#include "../../../Include/Script/SceneGamePlay/Management/script_state_machine.h"

namespace magiccup {

TotalManagement::TotalManagement() {
  m_key_input     = AddComponent<ScriptKeyInput>(*this);
  m_state_machine = AddComponent<ScriptStateMachine>(*this);
  m_data_container = AddComponent<ScriptDataContainer>(*this);
}

} /// ::magiccup namespace