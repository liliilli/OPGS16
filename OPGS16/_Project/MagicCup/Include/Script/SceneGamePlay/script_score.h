#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_SCORE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_SCORE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptUiScore final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiScore);

  void UpdateScore(int32_t score) noexcept;

private:
  void Initiate() override final;

  opgs16::element::canvas::CText* m_score = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_SCORE_H