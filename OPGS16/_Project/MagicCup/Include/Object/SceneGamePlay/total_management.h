#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_OVERALL_MANAGER_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_OVERALL_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {
class ScriptKeyInput;
class ScriptStateMachine;
class ScriptDataContainer;
}

namespace magiccup {

class TotalManagement final : public opgs16::element::CObject {
public:
  TotalManagement();

  inline static constexpr const char* s_object_name = "TotalMng";

private:
  ScriptKeyInput*     m_key_input = nullptr;
  ScriptStateMachine* m_state_machine = nullptr;
  ScriptDataContainer* m_data_container = nullptr;
};

} /// ::maigccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_OVERALL_MANAGER_H