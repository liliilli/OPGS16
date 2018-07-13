#ifndef MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H
#define MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {
class ChoiceList;
}

namespace magiccup {

class ScriptTitleSelect final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTitleSelect);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;
  void Destroy() override final;

  void GotoGameplay();
  void ExitGame();

  magiccup::ChoiceList* m_choice_list = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H