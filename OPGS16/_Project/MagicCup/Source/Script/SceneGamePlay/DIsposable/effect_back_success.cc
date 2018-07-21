
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Disposable/effect_back_success.h"

#include <Element/object.h>
#include <Manager/timer_manager.h>

#include "../../../../Include/Script/CanvasItem/script_background.h"

namespace magiccup {

void EffectBackgroundSuccess::Initiate() {
  m_script = GetBindObject().GetComponent<ScriptUiBackground>();

  OP16_TIMER_SET(m_timer_handle, 125, true, this,
                 &EffectBackgroundSuccess::CbTickEffect);
}

void EffectBackgroundSuccess::CbTickEffect() {
  --m_count;

  if (m_count <= 0) {
    m_script->SetTile(3);
    OP16_TIMER_STOP(m_timer_handle);
    GetBindObject().RemoveComponent<EffectBackgroundSuccess>();
    return;
  }

  if (m_is_turned_on) {
    m_script->SetTile(2);
  }
  else {
    m_script->SetTile(3);
  }

  m_is_turned_on = !m_is_turned_on;
}

} /// ::magiccup namespace