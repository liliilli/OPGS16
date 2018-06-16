
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

#include <Manager/scene_manager.h>
#include <Element/Canvas/text.h>

namespace debug::script {

void LifecycleManager::IncreaseObjectCount(uint32_t val) noexcept {
  m_count += val;
  m_text_ui->SetText(std::to_string(m_count));
}

void LifecycleManager::DecreaseObjectCount() noexcept {
  --m_count;
  m_text_ui->SetText(std::to_string(m_count));
}

void LifecycleManager::Initiate() {
  PUSH_LOG_INFO("LifecycleManager Initiate()");
  NEU_ASSERT(s_instantiated == false,
      "LifecycleManager is duplicated on scene.");

  s_instantiated = true;
}

void LifecycleManager::Start() {
  PUSH_LOG_INFO("LifecycleManager Start()");
  OP16_TIMER_SET(m_spawn_timer, 24, true, this, &LifecycleManager::SpawnObject);

  using opgs16::manager::scene::GetPresentScene;
  m_text_ui =
    static_cast<opgs16::element::canvas::CText*>(
    GetPresentScene()->GetGameObject("Canvas")->GetChild("Object")
  );
}

void LifecycleManager::Update(float delta_time) {
  // Do nothing.
}

void LifecycleManager::Destroy() {
  PUSH_LOG_INFO("LifecycleManager Destroy()");
}

void LifecycleManager::SpawnObject() {
  PUSH_LOG_INFO("LifecycleManager SpawnObject()");
  auto& object = GetBindObject();

  for (uint32_t i = 0; i < 1; ++i) {
    object.Instantiate<object::Hopping>("Hopping");
  }
  IncreaseObjectCount(1);
}

} /// ::debug::script