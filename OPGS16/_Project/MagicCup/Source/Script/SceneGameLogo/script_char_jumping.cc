
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGameLogo/script_char_jumping.h"

#include <Element/Canvas/text.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Internal/general_keyword.h"

namespace magiccup {

void ScriptCharJumping::Initiate() {
  using opgs16::element::canvas::CText;

  auto& obj = GetBindObject();

  auto ptr = obj.CreateGameObject<CText>("_", "");
  ptr->SetFontName(keyword::fon_3x5);
  ptr->SetFontSize(8);
  ptr->SetOrigin(IOriginable::Origin::CENTER_CENTER);
  ptr->SetAlignment(IAlignable::Alignment::CENTER);

  m_text = ptr;
  m_text->SetObjectActive(false);
  this->SetComponentActive(false);
}

void ScriptCharJumping::SetChar(char character) {
  m_text->SetText(std::string{character});
}

void ScriptCharJumping::ExecuteEffect(int32_t delay) {
  OP16_TIMER_SET(m_delay_timer, delay, false, this,
                 &ScriptCharJumping::EnableEffect);
}

void ScriptCharJumping::EnableEffect() {
  this->SetComponentActive(true);
  m_text->SetObjectActive(true);
}

void ScriptCharJumping::Update(float delta_time) {
  static constexpr int32_t interval = 250;
  static constexpr int32_t height = 16;
  static constexpr float frequency = 3.1415926535f / static_cast<float>(interval);

  m_elapsed += static_cast<int32_t>(delta_time * 1'000);
  auto& obj = GetBindObject();

  if (m_elapsed >= interval) {
    obj.SetLocalPosition({0, 0, 0});
    this->SetComponentActive(false);
  }
  else {
    obj.SetLocalPosition({0, height * sinf(frequency * m_elapsed), 0});
  }
}

} /// ::magiccup namespace