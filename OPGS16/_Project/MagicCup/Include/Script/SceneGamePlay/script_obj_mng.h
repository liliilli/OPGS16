#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

#include <vector>

namespace magiccup {

class ScriptObjectManagement final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptObjectManagement);

private:
  void Initiate() override final;

  std::vector<opgs16::element::CObject*> m_item_list;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H