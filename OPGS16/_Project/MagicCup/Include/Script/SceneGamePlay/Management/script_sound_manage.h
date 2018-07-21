#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_SOUND_MANAGE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_SOUND_MANAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptSoundManage final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptSoundManage);

private:
  void Initiate() override final;
  void Destroy() override final;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_SOUND_MANAGE_H