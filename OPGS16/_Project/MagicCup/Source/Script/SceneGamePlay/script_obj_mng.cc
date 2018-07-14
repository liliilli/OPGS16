
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_obj_mng.h"

#include <numeric>
#include <vector>
#include <random>

#include <Component/sprite_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Internal/moving_callback.h"

#include "../../../Include/Object/SceneGamePlay/object_cursor.h"
#include "../../../Include/Object/SceneGamePlay/total_management.h"
#include "../../../Include/Object/SceneGamePlay/Cup/obj_ball.h"
#include "../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"
#include "../../../Include/Object/SceneGamePlay/Cup/obj_cup_item.h"

#include "../../../Include/Script/SceneGamePlay/Cup/script_cup_number.h"
#include "../../../Include/Script/SceneGamePlay/Cup/script_item_movement.h"
#include "../../../Include/Script/SceneGamePlay/Management/script_state_machine.h"

namespace magiccup {

void ScriptObjectManagement::Initiate() {
  auto& obj = GetBindObject();

  const int32_t number = 3;

  float interval = static_cast<float>((256 - 64) / (number - 1));
  opgs16::DVector3 point = {-(256 - 64) / 2, 0.f, 0.f};

  for (int32_t i = 1; i <= number; ++i) {
    auto item = obj.Instantiate<ObjectCupItem>(ObjectCupItem::s_object_name);
    item->SetWorldPosition(point);
    point += opgs16::DVector3{interval, 0.f, 0.f};

    auto script = item->
        GetChild(ObjectCup::s_object_name)->GetComponent<ScriptCupNumbering>();
    if (script) {
      script->SetNumber(i);
    }

    m_item_list.push_back(item);
  }

  m_cursor = obj.Instantiate<ObjectCursor>(ObjectCursor::s_object_name);
  auto position = m_item_list[m_cursor_index]->GetWorldPosition();
  position += opgs16::DVector3{0.f, -16.f, 0.f};
  m_cursor->SetWorldPosition(position);
  m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
      SetComponentActivation(phitos::enums::EActivated::Disabled);

  SetComponentActivation(phitos::enums::EActivated::Disabled);
}

void ScriptObjectManagement::FirstStartObjectEffect() {
  m_ball_index = 1;
  const int32_t size = static_cast<int32_t>(m_item_list.size());

  for (int32_t i = 0; i < size; ++i) {
    m_item_list[i]->GetChild(ObjectBall::s_object_name)->
        SetActive(i == m_ball_index ? true : false);
  }

  SetComponentActivation(phitos::enums::EActivated::Activated);
}

void ScriptObjectManagement::StartObjectEffect() {
  SetComponentActivation(phitos::enums::EActivated::Activated);
}

void ScriptObjectManagement::Update(float delta_time) {
  SetComponentActivation(phitos::enums::EActivated::Disabled);
  OP16_TIMER_SET(m_effect_timer, 1'000, false, this,
                 &ScriptObjectManagement::ExecuteTransitionShaking);
}

void ScriptObjectManagement::ExecuteTransitionShaking() {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_state_machine) {
    m_state_machine = GetPresentScene()->
        GetGameObject(TotalManagement::s_object_name)->
        GetComponent<ScriptStateMachine>();
  }

  m_state_machine->TransitShaking();
}

void ScriptObjectManagement::ExecuteShaking(int32_t shaking_count) {
#ifdef false
  m_item_list[m_ball_index]->GetChild(ObjectBall::s_object_name)->
      SetActive(false);
#endif

  m_shaking_count_on_stage = shaking_count;
  OP16_TIMER_SET(m_shaking_timer, 2'000, true, this,
                 &ScriptObjectManagement::NextShake);
}

void ScriptObjectManagement::ExecuteTransitionSelect() {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_state_machine) {
    m_state_machine = GetPresentScene()->
        GetGameObject(TotalManagement::s_object_name)->
        GetComponent<ScriptStateMachine>();
  }

  m_state_machine->TransitSelect();
}

void ScriptObjectManagement::NextShake() {
  static std::random_device rd;
  static std::mt19937 rng(rd());

  --m_shaking_count_on_stage;
  if (m_shaking_count_on_stage == -1) {
    OP16_TIMER_STOP(m_shaking_timer);
    ExecuteTransitionSelect();
    return;
  }

  const int32_t size = static_cast<int32_t>(m_item_list.size());
  std::vector<int32_t> sequence;
  sequence.resize(size);

  std::iota(sequence.begin(), sequence.end(), 0);
  std::shuffle(sequence.begin(), sequence.end(), rng);

  const auto lhs = sequence[0], rhs = sequence[1];

  auto lhs_instance = m_item_list[lhs]->GetComponent<ScriptItemMovement>();
  auto rhs_instance = m_item_list[rhs]->GetComponent<ScriptItemMovement>();
  lhs_instance->SetMoveSetting(m_item_list[rhs]->GetWorldPosition(), 2'000);
  rhs_instance->SetMoveSetting(m_item_list[lhs]->GetWorldPosition(), 2'000);
  lhs_instance->SetMoveFunction(magiccup::DontMove);
  rhs_instance->SetMoveFunction(magiccup::DontMove);

  lhs_instance->ExecuteMoving();
  rhs_instance->ExecuteMoving();

  std::swap(m_item_list[lhs], m_item_list[rhs]);
  if (m_ball_index == lhs) {
    m_ball_index = rhs;
  }
  else if (m_ball_index == rhs) {
    m_ball_index = lhs;
  }
}

void ScriptObjectManagement::MoveCursorLeft() {
  if (m_cursor_index <= 0)
    return;

  auto position = m_item_list[--m_cursor_index]->GetWorldPosition();
  position += opgs16::DVector3{0.f, -16.f, 0.f};
  m_cursor->SetWorldPosition(position);
}

void ScriptObjectManagement::MoveCursorRight() {
  if (m_cursor_index == m_item_list.size() - 1)
    return;

  auto position = m_item_list[++m_cursor_index]->GetWorldPosition();
  position += opgs16::DVector3{0.f, -16.f, 0.f};
  m_cursor->SetWorldPosition(position);
}

void ScriptObjectManagement::MoveCursorSelectCup() {
  m_state_machine->TransitResult();
}

void ScriptObjectManagement::EnableCursor() {
  m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
      SetComponentActivation(phitos::enums::EActivated::Activated);
}

void ScriptObjectManagement::DisableCursor() {
  m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
      SetComponentActivation(phitos::enums::EActivated::Disabled);
}

void ScriptObjectManagement::ExecuteJudging() {
  OP16_TIMER_SET(m_effect_timer, 1'000, false, this,
                 &ScriptObjectManagement::Judge);
}

void ScriptObjectManagement::Judge() {
  if (m_ball_index != m_cursor_index) {
    m_state_machine->TransitGameState(EGameState::GameOver);
  }

  m_state_machine->TransitGameState(EGameState::NextStage);
}

} /// ::magiccup namespace