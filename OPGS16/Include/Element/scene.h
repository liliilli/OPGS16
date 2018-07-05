#ifndef OPGS16_ELEMENT_SCENE_H
#define OPGS16_ELEMENT_SCENE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/scene.h
///
/// @brief The file consist of basic scene API.
///
/// All derived class based on Scene class can be used
/// parameter of Application::PushScene().
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-19 Refactoring
/// 2018-03-11 Correcton of Object to opgs16::element::CObject.
/// 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
/// 2018-04-14 Add background color variable, and function.
/// 2018-04-14
/// Move ::Scene class to ::opgs16::element::Scene class.
/// Revised DEFINE name.
/// 2018-04-14 Rename ::opgs16::element::Scene to ::opgs16::element::CScene.
/// 2018-04-14 Move file to /System/Element/Public/ directory.
/// 2018-05-24 Implement object life cycle.
///

#include <memory>

#include <GL/glew.h>

/// ::opgs16::component::CScriptFrame
#include <Component/script_frame.h>
/// ::opgs16::element::CObject
#include <Element/object.h>

/// Type checking templates
#include <Helper/template.h>
/// Forward declaration
#include <opgs16fwd.h>
#include "Helper/Type/color.h"

namespace opgs16::element {

///
/// @class CScene
/// @brief Base scene interface.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
/// 2018-04-14 Add background color variable, and function.
/// 2018-04-14
/// Move ::Scene class to ::opgs16::element::Scene class. Revised DEFINE name.
///
class CScene {
private:
  using object_ptr    = std::unique_ptr<CObject>;
  using object_map    = std::unordered_map<std::string, object_ptr>;
  using _camera       = component::CCamera;

public:
  /// Must need virtual destrcutor
  virtual ~CScene() = default;

  virtual void Initiate() = 0;

  ///
  /// @brief
  /// The method update components movement, UI refresh, and so on.
  /// This method is able to overriding,
  /// but actual default behavior is just call ->Update() of m_object_list.
  ///
  virtual void Update(float delta_time);

  void Draw() {
    glClearColor(m_background_color.r,
                 m_background_color.g,
                 m_background_color.b,
                 m_background_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  };

	///
	/// @brief
	/// Instantiate game object in scene by moving object instance to container.
  /// If success, return pointer of moved object but nullptr if failed.
  ///
  /// @param[in] object_name Object name to speicify.
  /// @param[in] object_smtptr PRvalue unique smart pointer of object.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
  /// @todo Prevent object_name duplicated problem using object's mechanism.
  ///
	template <
    class _Ty,
    typename = std::enable_if_t<IsCObjectBase<_Ty> && IsCObjectSmtPtr<_Ty>>
  >
  _Ty* Instantiate(const std::string& object_name,
                   std::unique_ptr<_Ty>&& object_smtptr) {
    if (DoesObjectExist(object_name)) {
      PUSH_LOG_WARN_EXT("Object {0} already exist.", object_name);
      return nullptr;
    }

    auto result_pair = m_object_list.emplace(std::make_pair(
          object_name,
          std::move(object_smtptr))
    );
    NEU_ASSERT(result_pair.second, "Object did not be made proeprly.");

    object_ptr& object_ref = (result_pair.first)->second;
    object_ref->SetHash(object_name);
    return static_cast<_Ty*>(object_ref.get());
	}

  ///
  /// @brief
	/// Instantiate game object in scene by moving object instance to container.
  /// If success, return pointer of moved object but nullptr if failed.
  ///
  /// @param[in] object_name Object name to speicify.
  /// @param[in] obj Unique smart pointer reference of object.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
  /// @todo Prevent object_name duplicated problem using object's mechanism.
  ///
	template <
    class _Ty,
    typename = std::enable_if_t<IsCObjectBase<_Ty> && IsCObjectSmtPtr<_Ty>>
  >
  _Ty* Instantiate(const std::string& object_name, std::unique_ptr<_Ty>& obj) {
    if (DoesObjectExist(object_name)) {
      PUSH_LOG_WARN_EXT("Object {0} already exist.", object_name);
      return nullptr;
    }

    auto result_pair =
        m_object_list.emplace(std::make_pair(object_name, std::move(obj)));
    NEU_ASSERT(result_pair.second, "Object did not be made proeprly.");

    object_ptr& object_ref = (result_pair.first)->second;
    object_ref->SetHash(object_name);
    return static_cast<_Ty*>(object_ref.get());
	}

  ///
  /// @brief
	/// Instantiate game object in scene by specifing arguments
  /// to create in container.
  ///
  /// If success, return pointer of moved object but nullptr if failed.
  ///
  /// @param[in] object_name Object name to speicify.
  /// @param[in] args Argument values for constructor of each object type.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
  /// @todo Prevent object_name duplicated problem using object's mechanism.
  ///
  template <
    class _Ty,
    class... _Args,
    typename = std::enable_if_t<IsCObjectBase<_Ty>>
  >
  _Ty* Instantiate(const std::string& object_name, _Args&&... args) {
    if (DoesObjectExist(object_name)) {
      PUSH_LOG_WARN_EXT("Object {0} already exist.", object_name);
      return nullptr;
    }

    auto result_pair = m_object_list.emplace(std::make_pair(
        object_name,
        std::make_unique<_Ty>(std::forward<_Args>(args)...))
    );
    NEU_ASSERT(result_pair.second, "Object did not be made proeprly.");

    object_ptr& object_ref = (result_pair.first)->second;
    object_ref->SetHash(object_name);
    return static_cast<_Ty*>(object_ref.get());
	}

	///
	/// @brief Get object list loaded in scene.
	/// @return The reference of object list with hash_map.
	///
	object_map& GetObjectList();

	///
	/// @brief Get specific object with tag.
	///
	object_ptr& GetGameObject(const std::string& tag);

  ///
  /// @brief Set main camera of this scene, to display game scene.
  /// All object except for Canvas m_object_list (UI object) uses to main_camera to display.
  /// If main_camera value is nullptr, this means main_camera is detached.
  ///
  void SetMainCamera(_camera* const main_camera) {
    m_main_camera = main_camera;
  }

  ///
  /// @brief Get bound main camera.
  /// if main camera is not bound, return nullptr.
  ///
  const _camera* GetMainCamera() {
    return m_main_camera;
  }

  bool DoesObjectExist(const std::string& name) const {
    return m_object_list.find(name) != m_object_list.end();
  }

  ///
  /// @brief Get pointer of background color.
  /// @return Pointer of background color.
  ///
  DColor* BackgroundColor() noexcept {
    return &m_background_color;
  }

  ///
  /// @brief Set background color of present scene.
  ///
  void SetBackgroundColor(const DColor& color) noexcept {
    m_background_color = color;
  }

private:
  void ScriptInitiate(object_ptr& object_pointer);

  _camera* m_main_camera{ nullptr };
  DColor m_background_color = DColor::Black;
  object_map  m_object_list;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_SCENE_H

