
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Management/script_data.h"

#include <Manager/scene_manager.h>
#include <Element/object.h>

#include "../../../../Include/Internal/object_keyword.h"
#include "../../../../Include/Global/data_singleton.h"
#include "../../../../Include/Script/SceneGamePlay/script_ui_object.h"
#include "../../../../Include/Script/SceneGamePlay/script_score.h"
#include "../../../../Include/Script/SceneGamePlay/script_life.h"
#include "../../../../Include/Script/SceneGamePlay/script_stage.h"
#include "../../../../Include/Script/SceneGamePlay/script_obj_mng.h"
#include "../../../../Include/Object/SceneGamePlay/stage_obj_mng.h"
#include "../../../../Include/Object/SceneGamePlay/ui_object.h"

namespace magiccup {

void ScriptDataContainer::Initiate() {
  SetComponentActive(false);
}

int32_t ScriptDataContainer::GetScore() const noexcept {
  return m_score;
}

int32_t ScriptDataContainer::GetShakingInterval() noexcept {
  return m_shake_interval;
}

int32_t ScriptDataContainer::GetTimerLimitValue() noexcept {
  return m_initial_time_limit;
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

int32_t ScriptDataContainer::GetStage() const noexcept {
  return m_stage;
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

void ScriptDataContainer::SetTimerValue(int32_t value) noexcept {
  m_time_limit = value;
}

void ScriptDataContainer::RepeatStage() noexcept {
  m_time_limit = m_initial_time_limit;
  m_script_stage->ExecuteStageEffect(m_stage);
}

void ScriptDataContainer::IncrementStage() noexcept {
  using opgs16::manager::scene::GetPresentScene;

  static int32_t next_to_score = 0;
  static bool is_4th_initiated = false;
  static bool is_5th_initaited = false;

  if (!m_script_stage) {
    m_script_stage = GetPresentScene()->GetGameObject(name::canvas)->
        GetGameObject(GamePlayUi::s_obj_name)->
        GetComponent<ScriptUiObject>()->GetScriptStage();
  }

  if (!m_object_management) {
    m_object_management = GetPresentScene()->
        GetGameObject(StageObjectManagement::s_object_name)->
        GetComponent<ScriptObjectManagement>();
  }

  ++m_stage;

  if (m_stage > 1) {
    m_shaking += 1;

    if (!is_4th_initiated && m_stage == 7) {
      is_4th_initiated = true;
      m_object_management->IncreaseCup();
    }

    if (!is_5th_initaited && m_stage == 15) {
      is_5th_initaited = true;
      m_object_management->IncreaseCup();
    }

    if (m_shake_interval > m_shake_intv_lim) {
      m_shake_interval -= m_shake_intv_dec;
      if (m_shake_interval < m_shake_intv_lim)
        m_shake_interval = m_shake_intv_lim;
    }

    if (m_initial_time_limit > m_time_limt) {
      m_initial_time_limit -= m_time_intv_dec;
      if (m_initial_time_limit < m_time_limt)
        m_initial_time_limit = m_time_limt;
    }

    const int32_t score_instage = CalculateScore();
    next_to_score += score_instage;
    SetScore(GetScore() + score_instage);

    if (next_to_score >= m_extended) {
      next_to_score %= m_extended;

      if (GetLife() < m_life_lim) {
        SetLife(GetLife() + 1);
      }
    }
  }

  m_script_stage->ExecuteStageEffect(m_stage);
}

int32_t ScriptDataContainer::GetShakingCount() noexcept {
  return m_shaking;
}

int32_t ScriptDataContainer::CalculateScore() {
  static constexpr int32_t score_base = 1'000;
  const auto offset =
      static_cast<float>(m_time_limit) / m_initial_time_limit + 0.25f;

  return static_cast<int32_t>((score_base + 50 * (m_stage - 1)) * offset);
}

void ScriptDataContainer::StoreData() const {
  DScoreContainer item;
  item.name = "";
  item.score = GetScore();
  item.stage = GetStage();

  data::SaveAsRecentData(item);
  data::SaveRankData(item);
}

}

