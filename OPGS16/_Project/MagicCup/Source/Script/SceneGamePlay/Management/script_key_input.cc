
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Management/script_key_input.h"

#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/sound_manager.h>

#include "../../../../Include/Internal/general_keyword.h"

#include "../../../../Include/Object/SceneGamePlay/stage_obj_mng.h"
#include "../../../../Include/Script/SceneGamePlay/script_obj_mng.h"
#include "../../../../Include/Script/SceneGamePlay/Management/script_state_machine.h"

namespace magiccup {

void ScriptKeyInput::Start() {
  using opgs16::manager::scene::GetPresentScene;

  if (!is_start_initiated) {
    m_state_machine = GetBindObject().GetComponent<ScriptStateMachine>();
    m_obj_management = GetPresentScene()->
        GetGameObject(StageObjectManagement::s_object_name)->
        GetComponent<ScriptObjectManagement>();

    is_start_initiated = true;
  }
}

void ScriptKeyInput::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::sound::PlaySound;

  if (is_select_key_enabled) {
    if (IsKeyPressed(keyword::key_x)) {
      const auto key_val = GetKeyValue(keyword::key_x);

      if (key_val == 1.0f) {
        m_obj_management->MoveCursorRight();
      }
      else if (key_val == -1.0f) {
        m_obj_management->MoveCursorLeft();
      }
    }

    if (IsKeyPressed(keyword::key_enter)) {
      PlaySound(keyword::eff_decision);
      m_obj_management->MoveCursorSelectCup();
      DisableSelectKeyInput();
    }
  }
}

void ScriptKeyInput::EnableSelectKeyInput() {
  is_select_key_enabled = true;
}

void ScriptKeyInput::DisableSelectKeyInput() {
  is_select_key_enabled = false;
}

} /// ::magiccup namespace