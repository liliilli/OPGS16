
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Cup/script_item_movement.h"

#include <Element/object.h>

namespace magiccup {

void ScriptItemMovement::Initiate() {
  SetComponentActivation(phitos::enums::EActivated::Disabled);
}

void ScriptItemMovement::Update(float delta_time) {
  elapsed += (delta_time * 1000) / interval;

  if (!m_function) {
    PUSH_LOG_ERRO("ScriptItemMovement Moving function is not bind.");
    return;
  }

  auto& obj = GetBindObject();
  obj.SetWorldPosition(m_function(start_point, destination_point, elapsed));

  if (elapsed >= 1.0f) {
    obj.SetWorldPosition(destination_point);
    SetComponentActivation(phitos::enums::EActivated::Disabled);
    elapsed = 0.f;
  }
}

void ScriptItemMovement::SetMoveFunction(std::function<TCallback> function) {
  m_function = function;
}

void ScriptItemMovement::SetMoveSetting(opgs16::DVector3 destination,
                                        int32_t interval) {
  start_point = GetBindObject().GetWorldPosition();
  destination_point = destination;
  this->interval = interval;
  elapsed = 0.f;
}

void ScriptItemMovement::ExecuteMoving() {
  SetComponentActivation(phitos::enums::EActivated::Activated);
}

} /// ::magiccup namespace