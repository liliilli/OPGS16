#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
///
/// @file Manager/object_manager.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Refactoring.
/// 2018-03-11 Cope with ::element::CObject
/// 2018-05-25 Recode singleton class to namespace structure.
///
/// @todo Improve performance object destruction.
/// @todo Implement AABB 3D rendering sequence.
///

/// Header file
#include <Manager/object_manager.h>

#include <memory>
#include <list>
#include <stack>
#include <vector>

#include <Phitos/Dbg/assert.h>

#include <Component/Internal/aabb_renderer_base.h>
#include <Component/particle_emitter.h>
#include <Component/particle_spawner.h>

#include <Core/core_setting.h>
#include <Element/object.h>
#include <Headers/import_logger.h>

#include <Manager/scene_manager.h>
#include <Manager/setting_manager.h>
#include <Manager/Internal/error_message.h>
#include <Manager/Internal/flag.h>

using TObjectSmtPtr = std::unique_ptr<opgs16::element::CObject>;

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward Declaration
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// @brief
///
///
void DestroyObjects();

///
/// @brief
///
///
/// @param[in] ptr
///
///
void AddDestroyObject(TObjectSmtPtr& ptr);

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {
using opgs16::debug::EInitiated;
using opgs16::component::_internal::CPrivateAabbRendererBase;
using opgs16::component::CParticleEmitter;
using opgs16::component::CParticleSpawner;
using opgs16::component::_internal::CPrivateXyzAxisRenderer;
using TRenderedObjectSubList = std::list<opgs16::element::CObject*>;

EInitiated m_initiated = EInitiated::NotInitiated;

///
/// Rendering objects and common.
///
std::list<TObjectSmtPtr> m_destroy_candidates;
std::vector<TRenderedObjectSubList> m_rendering_list;

///
/// AABB rendering containers.
///
std::list<CPrivateAabbRendererBase*> m_aabb_2d_list;
std::list<CPrivateAabbRendererBase*> m_aabb_3d_list;
std::list<CParticleEmitter*>         m_emitter_list;
std::forward_list<CPrivateXyzAxisRenderer*> m_axis_list;

/// A neutered particle spawner list. (Could not spawn particle any more)
std::forward_list<std::unique_ptr<CParticleSpawner>> m_spawner_list;

///
/// @brief Private function for destruction object with recursive traverse.
///
bool pDestroyGameObjectRecursively(const opgs16::element::CObject& object,
                                   opgs16::element::CObject* root) {
  using TObjectMap = std::unordered_map<std::string, TObjectSmtPtr>;
  using TObjectItType = TObjectMap::iterator;
  using opgs16::manager::scene::GetPresentScene;

  std::stack<TObjectMap*> tree_object_list;
  if (!root) tree_object_list.emplace(GetPresentScene()->GetGameObjectList());
  else       tree_object_list.emplace(&root->GetGameObjectList());

  std::stack<TObjectItType> obj_it_list;
  obj_it_list.emplace(tree_object_list.top()->begin());

  while (!tree_object_list.empty()) {
    auto& object_list = *tree_object_list.top();
    auto  obj_it      =  obj_it_list.top();

    for (; obj_it != object_list.end(); ++obj_it) {
      if (obj_it->second == nullptr) continue;

      // object ptr is same to object refenrence address of obj_it?
      if (obj_it->second.get() == &object) {
        AddDestroyObject(obj_it->second);
        return true;
      }

      // Dig object tree down once more.
      if (auto& additional_list = obj_it->second->GetGameObjectList();
          !additional_list.empty()) {
        obj_it_list.pop();
        obj_it_list.emplace(++obj_it);
        obj_it_list.emplace(additional_list.begin());
        tree_object_list.emplace(&additional_list);
        break;
      }
    }

    // If failed to destroy object and no more children on this level.
    if (obj_it == object_list.end() &&
        &object_list == tree_object_list.top()) {
      tree_object_list.pop();
      obj_it_list.pop();
    }
  }

  return false;
}

///
/// @brief Private function for destruction candidate object with recursive traverse.
///
void pDestroyCandidateGameObjects() {
  using TObjectMap = std::unordered_map<std::string, TObjectSmtPtr>;
  using TObjectItType = TObjectMap::iterator;
  using opgs16::manager::scene::GetPresentScene;

  const int32_t size = static_cast<int32_t>(m_destroy_candidates.size());
  for (int32_t i = 0; i < size; ++i) {
    std::stack<TObjectMap*> tree_list;
    std::stack<TObjectItType> it_list;
    tree_list.emplace(GetPresentScene()->GetGameObjectList());
    it_list.emplace(tree_list.top()->begin());

    bool destroyed = false;
    while (destroyed == false && !tree_list.empty()) {
      auto object_list = tree_list.top();
      auto it = it_list.top();

      for (; it != object_list->end(); ++it) {
        if (it->second == nullptr) {
          object_list->erase(it);
          destroyed = true;
          break;
        };

        if (auto& additional_list = it->second->GetGameObjectList();
            !additional_list.empty()) {
          it_list.pop();
          it_list.emplace(++it);
          it_list.emplace(additional_list.begin());
          tree_list.emplace(&additional_list);
          break;
        }
      }

      if (destroyed == false && object_list == tree_list.top()) {
        tree_list.pop();
        it_list.pop();
      }
    }
  }
}

void pCallScriptDestroyFunction(opgs16::element::CObject* object) {
  using opgs16::component::CScriptFrame;
  // Get script component list from object which will be destroyed,
  // call DestroyGameObject() function.
  auto script_list = object->GetComponents<CScriptFrame>();
  for (auto script : script_list) {
    PUSH_LOG_INFO_EXT(
      "Object {0} called DestroyGameObject() function"
      "prior to being destroyed actually.", object->GetGameObjectName());
    script->Destroy();
  }
}

void pTraverseToBeDestroyedObject(opgs16::element::CObject* object) {
  auto& children_list = object->GetGameObjectList();
  for (auto& [string, smtptr] : children_list) {
    pTraverseToBeDestroyedObject(smtptr.get());
    pCallScriptDestroyFunction(smtptr.get());
  }
}

} /// unnamed namespace

namespace opgs16::manager::object {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
                debug::err_object_duplicated_init);
  m_initiated = EInitiated::Initiated;
  m_rendering_list.resize(setting::GetRenderingLayerNameListSize());
}

void Update(float delta_time) {
  if (!m_destroy_candidates.empty()) {
    DestroyObjects();
  }

  // Check spawner
  m_spawner_list.remove_if([](auto& spawner) { return spawner->IsSleep(); });
  for (auto& spawner : m_spawner_list) {
    spawner->Update(delta_time);
  }
}

void Render() {
  for (auto& list : m_rendering_list) {
    for (auto& item : list) {
      item->Draw();
    }
    list.clear();
  }

  if (opgs16::setting::IsEnableRenderingAabb()) {
    RenderAABB();
  }

  // Render particles without considering rendering layer.
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_PROGRAM_POINT_SIZE);
  for (auto& emitter : m_emitter_list) {
    emitter->Render();
  }
  m_emitter_list.clear();
  glDisable(GL_PROGRAM_POINT_SIZE);

  for (auto& xyz_axis : m_axis_list) {
    xyz_axis->Render();
  }
  m_axis_list.clear();

  glEnable(GL_DEPTH_TEST);
}

void RenderAABB() {
  glDisable(GL_DEPTH_TEST);
  for (auto& aabb_element : m_aabb_2d_list) aabb_element->Render();
  m_aabb_2d_list.clear();
#ifdef false
  for (auto& aabb_element : m_aabb_3d_list) aabb_element->Render();
  m_aabb_3d_list.clear();
#endif
  glEnable(GL_DEPTH_TEST);
}

bool DestroyGameObject(const element::CObject& object, element::CObject* root, bool is_recursive) {
  using TObjectMap = std::unordered_map<std::string, TObjectSmtPtr>;
  using TObjectItType = TObjectMap::iterator;

  if (!is_recursive) {
    TObjectMap* object_list = nullptr;

    if (!root)
      object_list = scene::GetPresentScene()->GetGameObjectList();
    else
      object_list = &root->GetGameObjectList();

    for (auto& [object_name, object_ptr] : *object_list) {
      if (object_ptr.get() == &object) {
        AddDestroyObject(object_ptr);
        return true;
      }
    }

    return false;
  }

  // Recursive code
  return pDestroyGameObjectRecursively(object, root);
}

void ClearDestroyCandidates() {
  m_destroy_candidates.clear();
}

void ClearRenderingList() {
  for (auto& sub_list : m_rendering_list)
    sub_list.clear();
}

void InsertRenderingObject(element::CObject* const object,
                           unsigned layer_index) {
  PHITOS_ASSERT(layer_index < m_rendering_list.size(),
      debug::err_object_out_of_bound_rendering_list);

  m_rendering_list[layer_index].emplace_back(object);
}

void InsertAABBInformation(CPrivateAabbRendererBase& aabb_component) {
  if (opgs16::setting::IsEnableRenderingAabb()) {

    switch (aabb_component.GetColliderType()) {
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    case component::_internal::EAabbColliderDmStyle::D2:
      m_aabb_2d_list.push_back(&aabb_component);
      break;
    case component::_internal::EAabbColliderDmStyle::D3:
      m_aabb_3d_list.push_back(&aabb_component);
      break;
    }

  }
}

void InsertParticleEmitter(component::CParticleEmitter& emitter_component) {
  m_emitter_list.push_back(&emitter_component);
}

void pBindRenderXyzAxisRenderer(component::_internal::CPrivateXyzAxisRenderer& component) {
  m_axis_list.emplace_front(&component);
}

void pMoveParticleSpawner(std::unique_ptr<CParticleSpawner>& particle_spawner) {
  particle_spawner->SetParticleSpawnSetting(false);
  m_spawner_list.emplace_front(std::move(particle_spawner));
}

} /// ::opgs16::manager::object

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Local functions
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

void AddDestroyObject(TObjectSmtPtr& ptr) {
  using opgs16::component::CParticleSpawner;
  using opgs16::manager::object::pMoveParticleSpawner;

  auto spawner_list = ptr->pPopComponents<CParticleSpawner>();
  for (auto& spawner : spawner_list) {
    pMoveParticleSpawner(spawner);
  }
  m_destroy_candidates.emplace_back(std::move(ptr));
}

void DestroyObjects() {
  pDestroyCandidateGameObjects();

  for (auto& object : m_destroy_candidates) {
    pTraverseToBeDestroyedObject(object.get());
    pCallScriptDestroyFunction(object.get());
  }

  m_destroy_candidates.clear();
}

