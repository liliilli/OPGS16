
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_background_part.h"

#include <Element/object.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Object/CanvasItem/background.h"
#include "../../../Include/Script/CanvasItem/script_background.h"

namespace magiccup {

void ScriptBackgroundPart::Initiate() {
  auto& obj = GetBindObject();

  auto back = obj.GetGameObject(UiBackground::s_object_name);
  if (!back) {
    PHITOS_UNEXPECTED_BRANCH();
  }

  m_background = back->GetComponent<ScriptUiBackground>();
  m_background->SetScale(0.25f, 0.25f);
  m_background->SetOffset(0.5f, -0.5f);
}

void ScriptBackgroundPart::OnEnabled() {
  OP16_TIMER_SET(m_timer_handle, 1'000, true, this,
                 &ScriptBackgroundPart::ChangeColorSequencially);
}

void ScriptBackgroundPart::OnDisabled() {
  OP16_TIMER_STOP(m_timer_handle);
}

void ScriptBackgroundPart::ChangeColorSequencially() {
  m_background_index += 1;
  m_background_index %= 5;
  m_background->SetTile(m_background_index);
}

} /// ::magiccup namespace