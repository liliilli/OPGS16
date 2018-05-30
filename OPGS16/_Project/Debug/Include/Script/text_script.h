#ifndef DEBUG_SCRIPT_TEXT_SCRIPT_H
#define DEBUG_SCRIPT_TEXT_SCRIPT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Script/text_script.h
///

/// ::opgs16::component::CScriptFrame
#include <Component/script_frame.h>

namespace debug::script {

OPGS16_CLASS_SCRIPT(TextScript)
public:
  TextScript(opgs16::element::CObject& bind_object) :
      CScriptFrame { bind_object } {};

private:
  void Initiate() override final;
  void Start() override final;
  void Update() override final;
  void Destroy() override final;

  bool is_created = false;
};

} /// ::debug::script namespace

#endif /// DEBUG_SCRIPT_TEXT_SCRIPT_H