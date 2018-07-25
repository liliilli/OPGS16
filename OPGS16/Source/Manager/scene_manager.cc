#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/scene_manager.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Refactoring.
///

/// Header file
#include <Manager/scene_manager.h>

/// ::opgs16::component::CCamera
#include <Component/camera.h>
/// ::opgs16::manager::MPhysicsManager
#include <Manager/physics_manager.h>
/// ::opgs16::manager::MTimerManager
#include <Manager/timer_manager.h>
/// ::opgs16::manager::MTextureMangaer
#include <Manager/texture_manager.h>
/// ::opgs16::manager::MObjectManager
#include <Manager/object_manager.h>
/// ::opgs16::manager::MShaderManager
#include <Manager/shader_manager.h>
/// ::opgs16::manager::MSoundManager
#include <Manager/sound_manager.h>
#include <Phitos/Dbg/assert.h>

//!
//! Datas
//!

namespace {

opgs16::manager::scene::TSceneStack m_scene_stack = {};

} /// unnamed namespace

//!
//! Implemenation
//!

namespace opgs16::manager::scene {

void PopScene() {
  entry::SetOnBeforeUpdateCallback(std::bind(&__::PrivatePopScene));
}

void InitiateTopScene() {
  return (*m_scene_stack.rbegin())->Initiate();
}

TSceneStack& GetLoadedSceneList() noexcept {
  return m_scene_stack;
}

element::CScene* GetPresentScene() {
  if (m_scene_stack.empty())
    return nullptr;

  return m_scene_stack.rbegin()->get();
}

bool IsSceneEmpty() noexcept {
  return m_scene_stack.empty();
}

const glm::mat4& GetPresentScenePvMatrix() {
  return (*m_scene_stack.rbegin())->GetMainCamera()->PvMatrix();
}

const glm::mat4& GetPresentMainCamProjMatrix() noexcept {
  return (*m_scene_stack.rbegin())->GetMainCamera()->ProjectionMatrix();
}

const glm::mat4& GetPresentMainCamViewMatrix() noexcept {
  return (*m_scene_stack.rbegin())->GetMainCamera()->ViewMatrix();
}

} /// ::opgs16::manager::scene namespace

namespace opgs16::manager::scene::__ {

TSceneStack& Get() {
  return m_scene_stack;
}

void ReleaseAllResources() {
  physics::Clear();
  timer::ClearAllTimers();
  sound::ReleaseAllSoundElements();
#ifdef false
  ShaderManager::Instance().Clear();   /*! Not implemented */
#endif
  TextureManager::Instance().Clear();  /*! Not precise? */
  object::ClearAll();
}

void PrivatePopScene() {
  if (m_scene_stack.empty()) {
    PHITOS_ASSERT(m_scene_stack.empty() == false,
                  "Scene stack must not be empty, at least have one.");
    return;
  }

  m_scene_stack.pop_back();
  ReleaseAllResources();

  if (m_scene_stack.size() >= 1)
    InitiateTopScene();
}

void Shutdown() {
  m_scene_stack.clear();
}

} /// ::opgs16::manager::scene::__ namespace