#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_LIFE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_LIFE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <vector>

#include <Component/script_frame.h>

namespace magiccup {

class ScriptUiLife final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiLife);

  void UpdateLife(int32_t life) noexcept;

private:
  void Initiate() override final;

  std::vector<opgs16::element::canvas::CImage*> m_life_container;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_LIFE_H