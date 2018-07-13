#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptUiStage final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiStage);

private:
  void Initiate() override final;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_STAGE_H