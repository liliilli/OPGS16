
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/GameOver/script_char.h"

#include <vector>
#include <Element/Canvas/text.h>

#include "../../../../Include/Object/SceneGamePlay/Gameover/ui_char.h"

namespace {

std::vector<char> m_chars = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'
};

} /// unnamed namespace

namespace magiccup {

void ScriptCharRoulette::Initiate() {
  using opgs16::element::canvas::CText;

  auto& obj = GetBindObject();
  m_obj = static_cast<UiCharRouletteObject*>(&obj);

  UpdateChar();
  RevertChracter();
}

std::string ScriptCharRoulette::GetCharacter() noexcept {
  return std::string{} + m_chars[m_character_index];
}

void ScriptCharRoulette::UpChar() {
  if (m_character_index == 0) {
    m_character_index = static_cast<int32_t>(m_chars.size() - 1);
  }
  else {
    m_character_index--;
  }

  UpdateChar();
}

void ScriptCharRoulette::DownChar() {
  if (m_character_index == static_cast<int32_t>(m_chars.size() - 1)) {
    m_character_index = 0;
  }
  else {
    m_character_index++;
  }

  UpdateChar();
}

void ScriptCharRoulette::UpdateChar() {
  if (!m_obj) {
    PHITOS_UNEXPECTED_BRANCH();
  }

  m_obj->SetText(std::string{} + m_chars[m_character_index]);
}

void ScriptCharRoulette::DecideCharacter() {
  m_obj->SetColor(opgs16::DColor::Gold);
}

void ScriptCharRoulette::RevertChracter() {
  m_obj->SetColor(opgs16::DColor::White);
}

} /// ::magiccup namespace