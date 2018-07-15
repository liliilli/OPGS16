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
/// 2018-07-09 Add name generation.
/// 2018-07-15 Refactoring, and rename Instantiate to CreateGameObject
///

#include <memory>

#include <GL/glew.h>
#include <Phitos/Dbg/assert.h>

/// ::opgs16::element::CObject
#include <Element/object.h>
/// Type checking templates
#include <Helper/template.h>
/// ::opgs16::DColor
#include <Helper/Type/color.h>

/// Forward declaration
#include <opgs16fwd.h>

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
  using TGameObjectSmtPtr = std::unique_ptr<CObject>;
  using TGameObjectMap    = std::unordered_map<std::string, TGameObjectSmtPtr>;
  using TNameCounterMap   = std::unordered_map<std::string, int32_t>;

public:
  virtual ~CScene() = default;

  virtual void Initiate() = 0;

  void Destroy();

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
  /// @param[in] object_smtptr Unique smart pointer reference of object.
  ///
  /// @return If success, raw pointer of moved object; otherwise nullptr.
  ///
	template <
    class TCObjectType,
    typename = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string& object_name,
                            std::unique_ptr<TCObjectType>& object_smtptr) {
    const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_object_list.try_emplace(
        object_final_name,
        nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::move(object_smtptr);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->SetHash(object_final_name);
    return static_cast<TCObjectType*>(object_ref.get());
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
  template <
    class TCObjectType,
    class... TConstructionArgs,
    typename = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string& object_name,
                            TConstructionArgs&&... args) {
    const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_object_list.try_emplace(
        object_final_name,
        nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::make_unique<TCObjectType>(
        std::forward<TConstructionArgs>(args)...);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->SetHash(object_final_name);
    return static_cast<TCObjectType*>(object_ref.get());
	}

	///
	/// @brief Get object list loaded in scene.
	/// @return The reference of object list with hash_map.
	///
	TGameObjectMap* GetGameObjectList() noexcept;

	///
	/// @brief Get specific object with tag.
	///
	CObject* GetGameObject(const std::string& object_name,
                         bool is_resursive = false);

  ///
  /// @brief Set main camera of this scene, to display game scene.
  /// All object except for Canvas m_object_list (UI object) uses to main_camera to display.
  /// If main_camera value is nullptr, this means main_camera is detached.
  ///
  void SetMainCamera(component::CCamera* main_camera) {
    m_main_camera = main_camera;
  }

  ///
  /// @brief Get bound main camera.
  /// if main camera is not bound, return nullptr.
  ///
  const component::CCamera* GetMainCamera() const noexcept {
    return m_main_camera;
  }

  bool IsObjectExist(const std::string& name) const {
    return m_object_list.find(name) != m_object_list.end();
  }

  ///
  /// @brief Get pointer of background color.
  /// @return Pointer of background color.
  ///
  DColor* GetBackgroundColor() noexcept {
    return &m_background_color;
  }

  ///
  /// @brief Set background color of present scene.
  ///
  void SetBackgroundColor(const DColor& color) noexcept {
    m_background_color = color;
  }

private:
  ///
  /// @brief Create child object name.
  /// @param[in] name
  /// @return
  ///
  inline std::string CreateChildTag(const std::string& name) noexcept {
    std::string item_tag = name;

    if (m_name_counter.find(name) != m_name_counter.end()) {
      auto& count = m_name_counter[name];
      count += 1;
      item_tag.append('_' + std::to_string(count));
    }
    else { m_name_counter[name] = 0; }

    return item_tag;
  }

  CObject* GetGameObjectResursively(const std::string& object_name) noexcept;

  component::CCamera* m_main_camera = nullptr;
  DColor m_background_color = DColor::Black;

  TGameObjectMap  m_object_list;
  TNameCounterMap m_name_counter;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_SCENE_H

