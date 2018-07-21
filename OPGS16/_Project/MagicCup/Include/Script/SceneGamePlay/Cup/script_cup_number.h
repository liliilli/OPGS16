#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_CUP_NUMBER_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_CUP_NUMBER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptCupNumbering final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptCupNumbering);

private:
  void Initiate() override final;

  opgs16::component::CSprite2DRenderer* m_texture = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_CUP_NUMBER_H