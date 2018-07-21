
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Disposable/effect_boom_script.h"

#include <Component/animator.h>
#include <Element/object.h>
#include <Manager/object_manager.h>

namespace magiccup {

void ScriptEffectBoom::Initiate() {
  using opgs16::component::CAnimator;

  auto& obj = GetBindObject();
  m_animator = obj.GetComponent<CAnimator>();
}

void ScriptEffectBoom::Update(float delta_time) {
  if (m_animator->IsSleep()) {
    opgs16::manager::object::Destroy(GetBindObject());
  }
}

} /// ::magiccup namespace