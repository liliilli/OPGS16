
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Script/lifecycle_manager.cc
///

/// Header file
#include "../../Include/Script/lifecycle_manager.h"

/// Enhanced assertion
#include <Helper/assert.h>
/// import logger
#include <Headers/import_logger.h>
/// ::opgs16::manager::MTimerManager
#include <Manager/timer_manager.h>

/// ::debug::object::Hopping for object to being spawned.
#include "../../Include/Object/hopping.h"

#undef GetObject

namespace debug::script {

void LifecycleManager::Initiate() {
  PUSH_LOG_INFO("LifecycleManager Initiate()");
  NEU_ASSERT(s_instantiated == false,
      "LifecycleManager is duplicated on scene.");

  s_instantiated = true;
}

void LifecycleManager::Start() {
  PUSH_LOG_INFO("LifecycleManager Start()");
  OP16_TIMER_SET(m_spawn_timer, 250, true, this, &LifecycleManager::SpawnObject);
}

void LifecycleManager::Update(float delta_time) {
  // Do nothing.
}

void LifecycleManager::Destroy() {
  PUSH_LOG_INFO("LifecycleManager Destroy()");
}

void LifecycleManager::SpawnObject() {
  PUSH_LOG_INFO("LifecycleManager SpawnObject()");
  auto& object = GetObject();
  object.Instantiate<object::Hopping>("Hopping");
}

} /// ::debug::script