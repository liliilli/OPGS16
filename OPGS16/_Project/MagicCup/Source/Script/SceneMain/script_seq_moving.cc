
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_seq_moving.h"

#include <Element/object.h>
#include <Manager/timer_manager.h>

namespace magiccup {

void ScriptSequenceMoving::Initiate() {
  SetComponentActive(false);
}

void ScriptSequenceMoving::ExecuteMovingEffect(int32_t time, bool is_vanished) {
  m_time = time;
  m_vanished = is_vanished;

  auto& obj = GetBindObject();

  if (m_vanished) {
    m_destination = opgs16::DVector3{-256.f, 0.f, 0.f};
  }
  else {
    obj.SetObjectActive(true);
    m_initial_position = obj.GetWorldPosition();
  }

  SetComponentActive(true);
}

void ScriptSequenceMoving::Update(float delta_time) {
  auto& obj = GetBindObject();
  m_elapsed += delta_time * 1'000;

  if (m_elapsed >= m_time) {
    obj.SetWorldPosition(m_destination);

    if (m_vanished) {
      obj.SetWorldPosition({256.f, 0.f, 0.f});
      obj.SetObjectActive(false);
    }

    GetBindObject().RemoveComponent<ScriptSequenceMoving>();
    return;
  }

  const auto offset = static_cast<float>(m_elapsed) / m_time;
  const auto pos = m_initial_position * (1 - offset) + m_destination * offset;
  obj.SetWorldPosition(pos);
}

} /// ::magiccup namespace