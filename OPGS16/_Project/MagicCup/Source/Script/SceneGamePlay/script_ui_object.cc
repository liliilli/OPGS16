
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_ui_object.h"

#include <Element/Canvas/canvas.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Internal/object_keyword.h"

#include "../../../Include/Object/CanvasItem/background.h"
#include "../../../Include/Object/SceneGamePlay/ui_life.h"
#include "../../../Include/Object/SceneGamePlay/ui_score.h"
#include "../../../Include/Object/SceneGamePlay/ui_timelimit.h"
#include "../../../Include/Object/SceneGamePlay/ui_stage.h"

#include "../../../Include/Script/CanvasItem/script_background.h"
#include "../../../Include/Script/SceneGamePlay/script_score.h"
#include "../../../Include/Script/SceneGamePlay/script_life.h"
#include "../../../Include/Script/SceneGamePlay/script_timelimit.h"
#include "../../../Include/Script/SceneGamePlay/script_stage.h"

namespace magiccup {

void ScriptUiObject::Initiate() {
  auto& obj = GetBindObject();

  m_score = obj.CreateGameObject<UiScore>(UiScore::s_object_name)->
      GetComponent<ScriptUiScore>();

  m_life = obj.CreateGameObject<UiLife>(UiLife::s_object_name)->
      GetComponent<ScriptUiLife>();

  m_timelimit = obj.CreateGameObject<UiTimelimit>(UiTimelimit::s_object_name)->
      GetComponent<ScriptUiTimelimit>();

  m_stage = obj.CreateGameObject<UiStage>(UiStage::s_object_name)->
      GetComponent<ScriptUiStage>();

  using opgs16::element::canvas::CCanvas;
  using opgs16::manager::scene::GetPresentScene;
  m_background = obj.CreateGameObject<UiBackground>(
      UiBackground::s_object_name,
      static_cast<CCanvas*>(GetPresentScene()->GetGameObject(name::canvas)))->
          GetComponent<ScriptUiBackground>();
  m_background->SetTile(3);
  m_background->SetOffset(0, 0);
}

} /// ::magiccup namespace