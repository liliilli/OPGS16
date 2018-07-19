
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGameLogo/script_logo_effect.h"

#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Object/SceneGameLogo/logo_char.h"
#include "../../../Include/Scene/scene_main.h"
#include "../../../Include/Script/SceneGameLogo/script_char_jumping.h"

namespace {

constexpr const char* logo_text = "TextLogo";

} /// unnamed namespace

namespace magiccup {

void ScriptLogoEffect::Initiate() {
  using opgs16::element::canvas::CText;

  constexpr const std::string_view logo_string = "MADE BY STUDIO-N";
  constexpr int32_t stirng_length = static_cast<int32_t>(logo_string.size());

  auto& obj = GetBindObject();
  m_logos.reserve(stirng_length);

  for (int32_t i = 0; i < stirng_length; ++i) {
    auto ptr = obj.CreateGameObject<UiLogoChar>(logo_text);
    ptr->SetWorldPosition({static_cast<float>(-40 + (5 * i)), -16.f, 0});

    auto script = ptr->GetComponent<ScriptCharJumping>();
    script->SetChar(logo_string[i]);

    m_logos.push_back(script);
  }

  this->SetComponentActive(false);
  OP16_TIMER_SET(m_timer, 1'000, false, this, &ScriptLogoEffect::ShowLogo);
}

void ScriptLogoEffect::Update(float delta_time) { }

void ScriptLogoEffect::Destroy() { }

void ScriptLogoEffect::ShowLogo() {
  static constexpr int32_t delay = 64;
  for (int i = 0; i < m_logos.size(); ++i) {
    m_logos[i]->ExecuteEffect(delay * (i + 1));
  }

  OP16_TIMER_STOP(m_timer);
  OP16_TIMER_SET(m_timer, 3'000, false, this, &ScriptLogoEffect::GotoMain);
}

void ScriptLogoEffect::GotoMain() {
  M_REPLACE_SCENE(SceneMain);
}

} /// ::magiccup namespace