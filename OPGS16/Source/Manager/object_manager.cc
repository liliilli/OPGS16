
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
///

/// Header file
#include <Manager/object_manager.h>

#include <memory>
#include <list>
#include <stack>
#include <vector>

/// ::opgs16::element::CObject
#include <Element/object.h>
/// Import logger
#include <Headers/import_logger.h>
/// Expanded assertion
#include <Helper/assert.h>
/// ::opgs16::manager::MSceneManager
#include <Manager/scene_manager.h>
/// ::opgs16::manager::MSettingManager
#include <Manager/setting_manager.h>
/// ::opgs16::debug error messages.
#include <Manager/Internal/error_message.h>
/// ::opgs16::manager::_internal boolean enum flags
#include <Manager/Internal/flag.h>

using object_ptr = std::unique_ptr<opgs16::element::CObject>;
using object_raw = opgs16::element::CObject * ;

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
void AddDestroyObject(object_ptr& ptr);

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;

EInitiated m_initiated = EInitiated::NotInitiated;

} /// unnamed namespace

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {

std::list<object_ptr> m_destroy_candidates;

std::vector<std::list<object_raw>> m_rendering_list;

} /// unnamed namespace

namespace opgs16::manager::object {

void Initiate() {
  NEU_ASSERT(m_initiated == EInitiated::NotInitiated,
      debug::err_object_duplicated_init);
  m_initiated = EInitiated::Initiated;

  m_rendering_list.resize(setting::GetRenderingLayerNameListSize());
}

void Update() {
  if (!m_destroy_candidates.empty())
    DestroyObjects();
}

void Render() {
  for (auto& list : m_rendering_list) {
    for (auto& item : list) {
      item->Draw();
    }
    list.clear();
  }
}

void Destroy(const element::CObject& object) {
  const auto hash_value = object.GetHash();

  using object_map = std::unordered_map<std::string, object_ptr>;
  using it_type = object_map::iterator;
  std::stack<object_map*> tree_list;
  std::stack<it_type> it_list;

  tree_list.emplace(&MSceneManager::Instance().PresentScene()->GetObjectList());
  it_list.emplace(tree_list.top()->begin());

  auto destroyed = false;
  while (!(destroyed || tree_list.empty())) {
    auto& object_list = *tree_list.top();
    auto it = it_list.top(); it_list.pop();

    for (; it != object_list.end(); ++it) {
      if (it->second) {   /*! If it is empty */
        if (hash_value == it->second->GetHash()) {
          AddDestroyObject(it->second);
          destroyed = true;
          break;
        }

        if (auto& additional_list = it->second->GetChildList(); !additional_list.empty()) {
          it_list.emplace(++it);
          tree_list.emplace(&additional_list);
          it_list.emplace(additional_list.begin());
          break;
        }
      }
    }

    if (!destroyed && it == object_list.end()) {
      tree_list.pop();
    }
  }
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
  NEU_ASSERT(layer_index < m_rendering_list.size(),
      debug::err_object_out_of_bound_rendering_list);

  m_rendering_list[layer_index].emplace_back(object);
}

} /// ::opgs16::manager::object

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Local functions
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

void AddDestroyObject(object_ptr& ptr) {
  m_destroy_candidates.emplace_back(std::move(ptr));
}

void DestroyObjects() {
  for (auto& object : m_destroy_candidates) {
    auto hash_value = object->GetHash();

    using object_map = std::unordered_map<std::string, object_ptr>;
    using it_type = object_map::iterator;
    std::stack<object_map*> tree_list;
    std::stack<it_type> it_list;

    tree_list.emplace(&opgs16::manager::MSceneManager::Instance().PresentScene()->GetObjectList());
    it_list.emplace(tree_list.top()->begin());

    bool destroyed = false;
    while (!(destroyed || tree_list.empty())) {
      auto& object_list = *tree_list.top();
      auto it = it_list.top();
      it_list.pop();

      for (; it != object_list.end(); ++it) {
        if (!(it->second)) {
          object_list.erase(it);
          destroyed = true;
          break;
        }

        if (auto& additional_list = it->second->GetChildList(); !additional_list.empty()) {
          it_list.emplace(++it);
          tree_list.emplace(&additional_list);
          it_list.emplace(additional_list.begin());
          break;
        }
      }

      if (!destroyed && it == object_list.end()) {
        tree_list.pop();
      }
    }

    // Get script component list from object which will be destroyed,
    // call Destroy() function.
    auto script_list = object->GetComponents<opgs16::component::CScriptFrame>();
    for (auto script : script_list) {
      PUSH_LOG_INFO_EXT(
        "Object {0} called Destroy() function"
        "prior to being destroyed actually.", object->GetObjectName());
      script->Destroy();
    }
  }

  m_destroy_candidates.clear();
}

