
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
  return m_scene_stack.rbegin()->Initiate();
}

TSceneStack& GetLoadedSceneList() noexcept {
  return m_scene_stack;
}

element::CScene* PresentScene() {
  if (m_scene_stack.empty())
    return nullptr;

  return m_scene_stack.rbegin()->get();
}

bool Empty() noexcept {
  return m_scene_stack.empty();
}

} /// ::opgs16::manager::scene namespace

namespace opgs16::manager::scene::__ {

TSceneStack& Get() {
  return m_scene_stack;
}

void ReleaseAllResources() {
  opgs16::manager::physics::Clear();
  MTimerManager::Instance().Clear();    /*! precise */
  MSoundManager::Instance().Clear();    /*! Not precise */
#ifdef false
  ShaderManager::Instance().Clear();   /*! Not implemented */
#endif
  TextureManager::Instance().Clear();  /*! Not precise? */
  manager::object::ClearAll();
}

void PrivatePopScene() {
  ReleaseAllResources();

  if (!m_scene_stack.empty())
    m_scene_stack.pop_back();

  if (m_scene_stack.size() >= 1)
    InitiateTopScene();
}

} /// ::opgs16::manager::scene::__ namespace