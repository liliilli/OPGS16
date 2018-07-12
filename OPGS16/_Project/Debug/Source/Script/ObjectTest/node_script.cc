
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/ObjectTest/node_script.h"

#include <Headers/import_logger.h>

namespace debug::script {

void NodeScript::Initiate() {
  PUSH_LOG_INFO("Initiate()");
}

void NodeScript::Start() {
  PUSH_LOG_INFO("Start()");
}

void NodeScript::OnEnabled() {
  PUSH_LOG_INFO("OnEnabled()");
}

void NodeScript::OnDisabled() {
  PUSH_LOG_INFO("OnDisabled()");
}

void NodeScript::Destroy() {
  PUSH_LOG_INFO("Destroy()");
}

} /// ::debug::script namespace