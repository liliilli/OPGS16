
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneLogo/script_logo.h"

#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Scene/scene_main.h"

namespace {

constexpr const char* logo_text = "TextLogo";

} /// unnamed namespace

namespace magiccup {

void ScriptLogo::Initiate() {
  using opgs16::element::canvas::CText;

  auto logo = GetBindObject().Instantiate<CText>(logo_text, "MADE BY STUDIO-N");
  logo->SetFontName("opSystem");
  logo->SetFontSize(8);
  logo->SetOrigin(IOriginable::Origin::CENTER_CENTER);
  logo->SetAlignment(IAlignable::Alignment::CENTER);
  logo->SetWorldPosition({0, -16.f, 0});
  logo->SetActive(false);

  m_logo = logo;
  OP16_TIMER_SET(m_timer, 1'000, false, this, &ScriptLogo::ShowLogo);

  SetComponentActivation(phitos::enums::EActivated::Disabled);
}

void ScriptLogo::Update(float delta_time) { }

void ScriptLogo::Destroy() {
  GetBindObject().DestroyChild(*m_logo);
}

void ScriptLogo::ShowLogo() {
  m_logo->SetActive(true);

  OP16_TIMER_STOP(m_timer);
  OP16_TIMER_SET(m_timer, 2'000, false, this, &ScriptLogo::GotoMain);
}

void ScriptLogo::GotoMain() {
  M_REPLACE_SCENE(SceneMain);
}

} /// ::magiccup namespace