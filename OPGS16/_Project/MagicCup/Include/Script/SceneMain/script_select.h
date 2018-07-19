#ifndef MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H
#define MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {
class ChoiceList;
}

namespace magiccup {

class ScriptTitleSelect final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTitleSelect);

  void EnableComponent();

private:
  void Initiate() override final;
  void Update(float delta_time) override final;
  void Destroy() override final;

  void GotoGameplay();
  void ExitGame();

  opgs16::element::CTimerHandle m_common_timer;

  void ExecuteGotoGamePlay();
  void ExecuteExit();

  magiccup::ChoiceList* m_choice_list = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCRIPTMAIN_SCRIPT_SELECT_H