
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Cup/script_cup_number.h"

#include <Component/sprite2d_renderer.h>

#include "../../../../Include/Object/SceneGamePlay/Cup/obj_number.h"

namespace magiccup {

void ScriptCupNumbering::Initiate() {
  SetComponentActive(false);

  auto& obj = GetBindObject();
  auto texture = obj.CreateGameObject<ObjectNumber>(ObjectNumber::s_object_name);
  m_texture = texture->GetComponent<opgs16::component::CSprite2DRenderer>();
}

} /// ::magiccup namespace