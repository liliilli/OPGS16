
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Management/script_data.h"

#include <Manager/scene_manager.h>

#include "../../../../Include/Object/SceneGamePlay/ui_object.h"

#include "../../../../Include/Script/SceneGamePlay/script_ui_object.h"
#include "../../../../Include/Script/SceneGamePlay/script_score.h"
#include "../../../../Include/Script/SceneGamePlay/script_life.h"
#include "../../../../Include/Script/SceneGamePlay/script_stage.h"

#include "../../../../Include/Internal/object_keyword.h"

namespace magiccup {

void ScriptDataContainer::Initiate() {
  using phitos::enums::EActivated;
  SetComponentActivation(EActivated::Disabled);
}

int32_t ScriptDataContainer::GetScore() const noexcept {
  return m_score;
}

void ScriptDataContainer::SetScore(int32_t value) noexcept {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_script_score) {
    m_script_score = GetPresentScene()->GetGameObject(name::canvas)->
        GetGameObject(GamePlayUi::s_obj_name)->
        GetComponent<ScriptUiObject>()->GetScriptScore();
  }

  m_score = value;
  m_script_score->UpdateScore(m_score);
}

int32_t ScriptDataContainer::GetLife() const noexcept {
  return m_life;
}

void ScriptDataContainer::SetLife(int32_t value) noexcept {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_script_life) {
    m_script_life = GetPresentScene()->GetGameObject(name::canvas)->
        GetGameObject(GamePlayUi::s_obj_name)->
        GetComponent<ScriptUiObject>()->GetScriptLife();
  }

  m_life = value;
  m_script_life->UpdateLife(m_life);
}

void ScriptDataContainer::IncrementStage() noexcept {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_script_stage) {
    m_script_stage = GetPresentScene()->GetGameObject(name::canvas)->
        GetGameObject(GamePlayUi::s_obj_name)->
        GetComponent<ScriptUiObject>()->GetScriptStage();
  }

  ++m_stage;
  if (m_stage != 1) {
    SetScore(GetScore() + 573);
    if (GetScore() >= 10000) {
      if (GetLife() < 5) {
        SetLife(GetLife() + 1);
      }
    }
  }

  m_script_stage->ExecuteStageEffect(m_stage);
}

}
