
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_stage.h"

#include <Element/Canvas/text.h>
#include <Component/font2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/SceneGamePlay/stage_obj_mng.h"
#include "../../../Include/Script/SceneGamePlay/script_obj_mng.h"

namespace magiccup {

void ScriptUiStage::Initiate() {
  using opgs16::element::canvas::CText;
  using opgs16::component::CFont2DRenderer;
  auto& obj = GetBindObject();

  auto text = obj.CreateGameObject<CText>("Text", "Stage 01");
  text->SetAlignment(IAlignable::Alignment::LEFT);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->GetComponent<CFont2DRenderer>()->SetRenderingLayer(3);
  text->SetObjectActive(false);

  m_stage_display = text;

  SetComponentActive(false);
}

void ScriptUiStage::ExecuteStageEffect(int32_t stage_value) {
  m_stage = stage_value;
  m_stage_display->SetObjectActive(true);
  m_stage_display->SetText("Stage : " + std::to_string(m_stage));

  OP16_TIMER_SET(m_effect_interval, 2'000, false, this,
                 &ScriptUiStage::FinishEffect);
}

void ScriptUiStage::FinishEffect() {
  using opgs16::manager::scene::GetPresentScene;

  if (!m_object_management) {
    m_object_management = GetPresentScene()->
        GetGameObject(StageObjectManagement::s_object_name)->
        GetComponent<ScriptObjectManagement>();
  }

  if (m_stage == 1 && !m_is_first_stage_executed) {
    m_is_first_stage_executed = true;
    m_object_management->FirstStartObjectEffect();
  }
  else {
    m_object_management->StartObjectEffect();
  }
}

} /// ::magiccup namespace