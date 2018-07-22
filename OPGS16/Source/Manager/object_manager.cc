
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

/// ::opgs16::element::CObject
#include <Element/object.h>
/// Import logger
#include <Headers/import_logger.h>
/// Expanded assertion
#include <Phitos/Dbg/assert.h>
/// ::opgs16::manager::MSceneManager
#include <Manager/scene_manager.h>
/// ::opgs16::manager::setting
#include <Manager/setting_manager.h>
/// ::opgs16::manager::shader namespace
#include <Manager/shader_manager.h>
/// ::opgs16::debug error messages.
#include <Manager/Internal/error_message.h>
/// ::opgs16::manager::_internal boolean enum flags
#include <Manager/Internal/flag.h>
///
#include <Shader/shader_wrapper.h>
/// ::opgs16::builtin::SAABB2DShader
#include <Shader/Default/aabb_2d_line.h>

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

/// AABB rendering containers.
///
std::list<opgs16::DAABBInfoBox> m_aabb_2d_list;
std::list<opgs16::DAABBInfoBox> m_aabb_3d_list;

/// AABB GetWrapper
opgs16::element::CShaderWrapper m_aabb_2d_wrapper;
opgs16::element::CShaderWrapper m_aabb_3d_wrapper;

} /// unnamed namespace

namespace opgs16::manager::object {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      debug::err_object_duplicated_init);
  m_initiated = EInitiated::Initiated;

  m_rendering_list.resize(setting::GetRenderingLayerNameListSize());

  m_aabb_2d_wrapper.SetShader(
      shader::GetShader(builtin::shader::SAABB2DShader::s_shader_name));
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

void RenderAABB() {
  glDisable(GL_DEPTH_TEST);

#ifdef false
  if (!m_aabb_2d_list.empty()) {
    glLineWidth(2.f);
    m_aabb_2d_wrapper.SetUniformValue("uColor", glm::vec3{0, 1, 0});
    m_aabb_2d_wrapper.UseShader();
    for (const auto& _2d_aabb : m_aabb_2d_list) {
      const auto rectangle_vectors = _2d_aabb.GetVertexPoints();
      m_vao.Map(rectangle_vectors);
      glBindVertexArray(m_vao.GetVao());
      glDrawArrays(GL_LINE_LOOP, 0, 4);
    }

    glBindVertexArray(0);
    m_aabb_2d_list.clear();
  }

  if (!m_aabb_3d_list.empty()) {
    for (const auto& _3d_aabb : m_aabb_3d_list) {
      PHITOS_NOT_IMPLEMENTED_ASSERT();
    }

    m_aabb_3d_list.clear();
  }
#endif

  glEnable(GL_DEPTH_TEST);
}

void Destroy(const element::CObject& object, element::CObject* root) {
  using TObjectMap = std::unordered_map<std::string, object_ptr>;
  using TObjectItType = TObjectMap::iterator;
  std::stack<TObjectMap*> tree_list;
  std::stack<TObjectItType> it_list;

  if (!root)
    tree_list.emplace(scene::GetPresentScene()->GetGameObjectList());
  else
    tree_list.emplace(&root->GetGameObjectList());

  it_list.emplace(tree_list.top()->begin());
  bool destroyed = false;
  const auto hash_value = object.GetHash();

  while (!destroyed && !tree_list.empty()) {
    auto& object_list = *tree_list.top();
    auto it = it_list.top();

    for (; it != object_list.end(); ++it) {
      if (!it->second)
        continue;

      if (hash_value == it->second->GetHash() &&
          it->second.get() == &object) {
        AddDestroyObject(it->second);
        destroyed = true;
        break;
      }

      if (auto& additional_list = it->second->GetGameObjectList();
          !additional_list.empty()) {
        it_list.pop();
        it_list.emplace(++it);
        it_list.emplace(additional_list.begin());
        tree_list.emplace(&additional_list);
        break;
      }
    }

    if (it == object_list.end() && &object_list == tree_list.top()) {
      tree_list.pop();
      it_list.pop();
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
  PHITOS_ASSERT(layer_index < m_rendering_list.size(),
      debug::err_object_out_of_bound_rendering_list);

  m_rendering_list[layer_index].emplace_back(object);
}

void InsertAABBInformation(EAABBStyle mode, const DAABBInfoBox& aabb_box) {
  switch (mode) {
  case EAABBStyle::_2D:
    m_aabb_2d_list.emplace_front(aabb_box);
    break;
  case EAABBStyle::_3D:
    m_aabb_3d_list.emplace_front(aabb_box);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::opgs16::manager::object

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Local functions
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

void AddDestroyObject(object_ptr& ptr) {
  m_destroy_candidates.emplace_back(std::move(ptr));
}

void DestroyObjects() {
  const int32_t size = static_cast<int32_t>(m_destroy_candidates.size());
  for (int32_t i = 0; i < size; ++i) {
    using object_map = std::unordered_map<std::string, object_ptr>;
    using it_type = object_map::iterator;
    std::stack<object_map*> tree_list;
    std::stack<it_type> it_list;

    tree_list.emplace(opgs16::manager::scene::GetPresentScene()->GetGameObjectList());
    it_list.emplace(tree_list.top()->begin());

    bool destroyed = false;
    while (!destroyed && !tree_list.empty()) {
      auto& object_list = *tree_list.top();
      auto it = it_list.top();

      for (; it != object_list.end(); ++it) {
        if (!it->second) {
          object_list.erase(it);
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

      if (!destroyed && &object_list == tree_list.top()) {
        tree_list.pop();
        it_list.pop();
      }
    }
  }

  for (auto& object : m_destroy_candidates) {
    // Get script component list from object which will be destroyed,
    // call Destroy() function.
    auto script_list = object->GetComponents<opgs16::component::CScriptFrame>();
    for (auto script : script_list) {
      PUSH_LOG_INFO_EXT(
        "Object {0} called Destroy() function"
        "prior to being destroyed actually.", object->GetGameObjectName());
      script->Destroy();
    }
  }
  m_destroy_candidates.clear();
}

