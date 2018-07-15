#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_UP_OBJECT_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_UP_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {
class ScriptUiScore;
class ScriptUiLife;
class ScriptUiTimelimit;
class ScriptUiStage;
class ScriptUiBackground;
}

namespace magiccup {

class ScriptUiObject final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiObject);

  inline ScriptUiScore* GetScriptScore() noexcept {
    return m_score;
  }

  inline ScriptUiLife* GetScriptLife() noexcept {
    return m_life;
  }

  inline ScriptUiTimelimit* GetScriptTimelimit() noexcept {
    return m_timelimit;
  }

  inline ScriptUiStage* GetScriptStage() noexcept {
    return m_stage;
  }

  inline ScriptUiBackground* GetBackground() noexcept {
    return m_background;
  }

private:
  void Initiate() override final;

  ScriptUiScore*  m_score = nullptr;
  ScriptUiLife*   m_life = nullptr;
  ScriptUiTimelimit* m_timelimit = nullptr;
  ScriptUiStage*  m_stage = nullptr;
  ScriptUiBackground* m_background = nullptr;
};

}

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_UP_OBJECT_H