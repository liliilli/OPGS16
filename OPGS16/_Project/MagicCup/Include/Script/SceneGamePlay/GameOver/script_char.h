#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_GAMEOVER_SCRIPT_CHAR_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_GAMEOVER_SCRIPT_CHAR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

//!
//! Forward declaration
//!

namespace magiccup {
class UiCharRouletteObject;
}

//!
//! Implementation
//!

namespace magiccup {

class ScriptCharRoulette final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptCharRoulette);

  std::string GetCharacter() noexcept;

  void UpChar();

  void DownChar();

  void DecideCharacter();

  void RevertChracter();

private:
  void Initiate() override final;

  void UpdateChar();

  UiCharRouletteObject* m_obj = nullptr;
  int32_t m_character_index = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_GAMEOVER_SCRIPT_CHAR_H