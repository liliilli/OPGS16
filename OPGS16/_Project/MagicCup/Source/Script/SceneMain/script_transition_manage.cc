
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_transition_manage.h"

#include <Element/object.h>
#include <Manager/input_manager.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Object/SceneMain/ui_title_part.h"
#include "../../../Include/Object/SceneMain/ui_scoreboard_part.h"

namespace magiccup {

void ScriptTransitionManagement::SwitchOnSequence() {
  this->SetComponentActive(true);
}

void ScriptTransitionManagement::Initiate() {
  auto& obj = GetBindObject();

  m_sequences[0] = obj.GetGameObject(UiTitlePart::s_object_name);
  m_sequences[1] = obj.GetGameObject(UiScoreboardPart::s_object_name);
  m_sequences[2] = nullptr;

  m_sequences[0]->SetObjectActive(true);
  this->SetComponentActive(false);
}

void ScriptTransitionManagement::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;
  using opgs16::manager::input::GetKeyValue;

  static constexpr int32_t m_till_focus = 10'000;
  static constexpr int32_t m_next_seqen = 5'000;

  m_elapsed += static_cast<int32_t>(delta_time * 1000);

  if (m_on_focused) {
    if (m_elapsed >= m_next_seqen) {
      m_elapsed = 0;

      if (auto present_seq = m_sequences[m_sequence]) {
        present_seq->SetObjectActive(false);
        present_seq->SetWorldPosition({256, 0, 0});
      }

      m_sequence += 1;
      m_sequence %= m_seq_size;
      if (auto next_seq = m_sequences[m_sequence]) {
        next_seq->SetObjectActive(true);
        next_seq->SetWorldPosition({0, 0, 0});
      }
      return;
    }

    if (IsKeyPressed(keyword::key_enter)) {
      m_on_focused = false;
      m_elapsed = 0;

      if (auto present_seq = m_sequences[m_sequence]) {
        present_seq->SetObjectActive(false);
        present_seq->SetWorldPosition({256, 0, 0});
      }

      m_sequence = 0;
      if (auto next_seq = m_sequences[m_sequence]) {
        next_seq->SetObjectActive(true);
        next_seq->SetWorldPosition({0, 0, 0});
      }
    }

    // m_on_focused == true end.
  }
  else {
    if (m_elapsed >= m_till_focus) {
      m_on_focused = true;
      m_elapsed = 0;

      if (auto present_seq = m_sequences[m_sequence]) {
        present_seq->SetObjectActive(false);
        present_seq->SetWorldPosition({ 256, 0, 0 });
      }

      m_sequence += 1;
      m_sequence %= m_seq_size;
      if (auto next_seq = m_sequences[m_sequence]) {
        next_seq->SetObjectActive(true);
        next_seq->SetWorldPosition({ 0, 0, 0 });
      }
    }
  }
}

} /// ::magiccup namespace