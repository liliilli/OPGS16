
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/Scoreboard/rank_text_divingin.h"

#include <Element/object.h>
#include <Manager/timer_manager.h>

namespace magiccup {

void ScriptRankTextDivingIn::Initiate() {
  SetComponentActive(false);
}

void ScriptRankTextDivingIn::ExecuteMovingEffect(
    int32_t delay,
    int32_t time,
    const opgs16::DVector3& position) {
  m_delay = delay;
  m_time = time;
  m_destination = position;

  if (m_delay == 0) {
    CbExecute();
  }
  else {
    OP16_TIMER_SET(m_timer, m_delay, false, this,
                   &ScriptRankTextDivingIn::CbExecute);
  }
}

void ScriptRankTextDivingIn::CbExecute() {
  SetComponentActive(true);
}

void ScriptRankTextDivingIn::Update(float delta_time) {
  auto& obj = GetBindObject();
  if (!m_is_initialized) {
    m_initial_position = obj.GetWorldPosition();
    m_is_initialized = true;
  }

  m_elapsed += delta_time * 1'000;
  if (m_elapsed >= m_time) {
    obj.SetWorldPosition(m_destination);
    GetBindObject().RemoveComponent<ScriptRankTextDivingIn>();
    return;
  }

  const auto offset = static_cast<float>(m_elapsed) / m_time;
  const auto pos = m_initial_position * (1 - offset) + m_destination * offset;
  obj.SetWorldPosition(pos);
}

} /// ::magiccup namespace