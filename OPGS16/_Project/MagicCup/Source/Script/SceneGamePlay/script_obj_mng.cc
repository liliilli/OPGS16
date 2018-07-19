
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

#include <Component/sprite2d_renderer.h>
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
    auto item = obj.CreateGameObject<ObjectCupItem>(ObjectCupItem::s_object_name);
    item->SetWorldPosition(point);
    point += opgs16::DVector3{interval, 0.f, 0.f};

    auto script = item->
        GetGameObject(ObjectCup::s_object_name)->GetComponent<ScriptCupNumbering>();
    if (script) {
      script->SetNumber(i);
    }

    m_item_list.push_back(item);
  }

  m_cursor = obj.CreateGameObject<ObjectCursor>(ObjectCursor::s_object_name);
  auto position = m_item_list[m_cursor_index]->GetWorldPosition();
  position += opgs16::DVector3{0.f, -16.f, 0.f};
  m_cursor->SetWorldPosition(position);
  m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
      SetComponentActive(false);

  SetComponentActive(false);
}

void ScriptObjectManagement::Start() {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_state_machine) {
    m_state_machine = GetPresentScene()->
        GetGameObject(TotalManagement::s_object_name)->
        GetComponent<ScriptStateMachine>();
  }
}

void ScriptObjectManagement::FirstStartObjectEffect() {
  m_ball_index = 1;
  const int32_t size = static_cast<int32_t>(m_item_list.size());

  for (int32_t i = 0; i < size; ++i) {
    m_item_list[i]->GetGameObject(ObjectBall::s_object_name)->
        SetObjectActive(i == m_ball_index ? true : false);
  }

  m_state = EState::StartEffect;
  SetComponentActive(true);
}

void ScriptObjectManagement::StartObjectEffect() {
  m_state = EState::IntermissionDownEffect;
  SetComponentActive(true);
}

void ScriptObjectManagement::Update(float delta_time) {
  using phitos::enums::EActivated;

  static constexpr float start_eff_int = 1.5f;
  static constexpr float eff_int = 1.0f;
  static constexpr float a = 96.f;
  static constexpr float o = 64.f;
  const int32_t size = static_cast<int32_t>(m_item_list.size());

  static float elapsed = 0.f;
  elapsed += delta_time;

  switch (m_state) {
  case EState::StartEffect: {
      if (elapsed >= start_eff_int) {
        elapsed = 0.f;
        m_state = EState::Idle;

        for (int32_t i = 0; i < size; ++i) {
          m_item_list[i]->GetGameObject(ObjectCup::s_object_name)->
              SetWorldPosition({0, a, 0});
        }

        SetComponentActive(false);
        OP16_TIMER_SET(m_effect_timer, 1'000, false, this,
                       &ScriptObjectManagement::ExecuteEffectCupDown);
        break;
      }

      const auto h = (ObjectCup::s_initial_y_pos - a) * 0.25f;
      const auto x = std::powf(elapsed - start_eff_int, 2);
      const float new_y = h * x + a;

      for (int32_t i = 0; i < size; ++i) {
        m_item_list[i]->GetGameObject(ObjectCup::s_object_name)->
            SetWorldPosition({0, new_y, 0});
      }
    } break;
  case EState::JudgeUpEffect: {
      if (elapsed >= eff_int) {
        elapsed = 0.f;
        m_state = EState::Idle;
        SetComponentActive(false);
        OP16_TIMER_SET(m_effect_timer, 1'000, false, this,
                       &ScriptObjectManagement::Judge);
        break;
      }

      const float off1 = elapsed / eff_int;
      const float off2 = (eff_int - elapsed) / eff_int;
      for (int32_t i = 0; i < size; ++i) {
        m_item_list[i]->GetGameObject(ObjectCup::s_object_name)->
            SetWorldPosition({0, a * off1 + 64.f * off2, 0});
      }
    }
    break;
  case EState::IntermissionDownEffect: {
      if (elapsed >= eff_int) {
        elapsed = 0.f;
        m_state = EState::Idle;
        SetComponentActive(false);
        OP16_TIMER_SET(m_effect_timer, 1'000, false, this,
                       &ScriptObjectManagement::ExecuteTransitionShaking);
        break;
      }

      const float off1 = (eff_int - elapsed) / eff_int;
      const float off2 = elapsed / eff_int;
      for (int32_t i = 0; i < size; ++i) {
        m_item_list[i]->GetGameObject(ObjectCup::s_object_name)->
            SetWorldPosition({0, a * off1 + 64.f * off2, 0});
      }
    } break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

void ScriptObjectManagement::ExecuteEffectCupDown() {
  using phitos::enums::EActivated;
  m_state = EState::IntermissionDownEffect;
  SetComponentActive(true);
}

void ScriptObjectManagement::ExecuteShaking(int32_t shaking_count) {
  m_shaking_count_on_stage = shaking_count;
  OP16_TIMER_SET(m_shaking_timer, 1'000, true, this, &ScriptObjectManagement::NextShake);
}

void ScriptObjectManagement::ExecuteTransitionShaking() {
  using opgs16::manager::scene::GetPresentScene;
  m_state_machine->TransitShaking();
}

void ScriptObjectManagement::ExecuteTransitionSelect() {
  using opgs16::manager::scene::GetPresentScene;
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
  lhs_instance->SetMoveSetting(m_item_list[rhs]->GetWorldPosition(), 1'000);
  rhs_instance->SetMoveSetting(m_item_list[lhs]->GetWorldPosition(), 1'000);

  if (lhs < rhs) {
    lhs_instance->SetMoveFunction(magiccup::BazierMoveLtR);
    rhs_instance->SetMoveFunction(magiccup::BazierMoveRtL);
  }
  else {
    rhs_instance->SetMoveFunction(magiccup::BazierMoveLtR);
    lhs_instance->SetMoveFunction(magiccup::BazierMoveRtL);
  }

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
  SetComponentActive(true);;
}

void ScriptObjectManagement::DisableCursor() {
  m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
      SetComponentActive(false);
}

void ScriptObjectManagement::ExecuteJudging() {
 using phitos::enums::EActivated;
  m_state = EState::JudgeUpEffect;
  SetComponentActive(true);;
}

void ScriptObjectManagement::Judge() {
  if (m_ball_index != m_cursor_index) {
    m_state_machine->TransitGameState(EGameState::GameOver);
  }

  m_state_machine->TransitGameState(EGameState::NextStage);
}

} /// ::magiccup namespace