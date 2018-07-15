
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../../Include/Script/SceneGamePlay/Management/script_state_machine.h"

#include <Element/object.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include "../../../../Include/Internal/object_keyword.h"
#include "../../../../Include/Object/SceneGamePlay/stage_obj_mng.h"
#include "../../../../Include/Object/SceneGamePlay/ui_object.h"
#include "../../../../Include/Script/SceneGamePlay/script_obj_mng.h"
#include "../../../../Include/Script/SceneGamePlay/script_timelimit.h"
#include "../../../../Include/Script/SceneGamePlay/script_ui_object.h"
#include "../../../../Include/Script/SceneGamePlay/Management/script_data.h"
#include "../../../../Include/Script/SceneGamePlay/Management/script_key_input.h"
#include "../../../../Include/Scene/scene_main.h"

namespace magiccup {

void ScriptStateMachine::Initiate() {
  m_state = EGameState::Initialize;
}

void ScriptStateMachine::Start() {
  auto& obj = GetBindObject();

  m_data = obj.GetComponent<ScriptDataContainer>();
  using opgs16::manager::scene::GetPresentScene;

  if (!m_object_management) {
    m_object_management = GetPresentScene()->
        GetGameObject(StageObjectManagement::s_object_name)->
        GetComponent<ScriptObjectManagement>();
  }

  if (!m_input) {
    m_input = obj.GetComponent<ScriptKeyInput>();
  }

  if (!m_timelimit) {
    m_timelimit = GetPresentScene()->GetGameObject(name::canvas)->
        GetGameObject(GamePlayUi::s_obj_name)->
        GetComponent<ScriptUiObject>()->
        GetScriptTimelimit();
  }

  TransitGameState(EGameState::GameStart);
}

void ScriptStateMachine::TransitShaking() {
  TransitGameState(EGameState::Shaking);
}

void ScriptStateMachine::TransitSelect() {
  TransitGameState(EGameState::Select);
}

void ScriptStateMachine::TransitResult() {
  TransitGameState(EGameState::Result);
}

void ScriptStateMachine::TransitGameState(EGameState new_state) {
  const auto previous = m_state;
  m_state = new_state;

  switch (previous) {
  case EGameState::Initialize:
    if (new_state == EGameState::GameStart) {
      m_data->SetScore(0);
      m_data->SetLife(3);
      m_data->IncrementStage();
    }
    break;
  case EGameState::GameStart:
  case EGameState::NextStage:
    if (new_state == EGameState::Shaking) {
      m_object_management->ExecuteShaking(m_data->GetShakingCount());
    }
    break;
  case EGameState::Shaking:
    if (new_state == EGameState::Select) {
      m_object_management->EnableCursor();
      m_input->EnableSelectKeyInput();
      m_timelimit->ExecuteTimeLimit(5'000);
    }
    break;
  case EGameState::Select:
    if (new_state == EGameState::Result) {
      m_object_management->ExecuteJudging();
      m_timelimit->HaltTimeLimit();
    }
    else if (new_state == EGameState::GameOver) {
      using opgs16::manager::scene::GetPresentScene;
      GetPresentScene()->SetBackgroundColor(opgs16::DColor::Red);
      OP16_TIMER_SET(m_gameover_effect_timer, 2'000, false, this,
                     &ScriptStateMachine::ReturnToTitle);
    }
    break;
  case EGameState::Result:
    if (new_state == EGameState::GameOver) {
      using opgs16::manager::scene::GetPresentScene;
      GetPresentScene()->SetBackgroundColor(opgs16::DColor::Red);
      OP16_TIMER_SET(m_gameover_effect_timer, 2'000, false, this,
                     &ScriptStateMachine::ReturnToTitle);
    }
    else if (new_state == EGameState::NextStage) {
      m_data->IncrementStage();
    }
    break;
  case EGameState::GameOver:
    break;
  case EGameState::GameEnd:
    break;
  default: break;
  }
}

void ScriptStateMachine::ReturnToTitle() {
  M_REPLACE_SCENE(SceneMain);
}

} /// ::magiccup namespace