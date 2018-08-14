﻿#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H
#define OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element\object.h
///
/// @brief
/// The file consist of basic scene API.
///
/// Object abstract class stores common values and
/// inheritable by derived each object class.
/// When using each m_object_list you have to use this type as
/// a storing type to avoid cracking of polymorphism.
///
/// @author Jongmin Yun

/// @log
/// 2018-02-19 Refactoring. Remove Draw(ShaderNew) obsolete not helpful method. Yeah!
/// 2018-02-19
/// Add GetParentPosition(). Remove virtual property from Draw() and Render()
/// virtual methods. and Remove virtual property from Update() instead of
/// adding LocalUpdate() method which replaces Update() override.
/// 2018-02-23
/// Add hierarchy rotation and scaling option. (mechanism is not implemented yet)
/// 2018-03-05 Add member function related to controlling rendering layer.
/// 2018-03-11 Move contents into ::opgs16::element namespace.
/// 2018-04-16 Add rotation (parent, world) get/set functions.
/// 2018-04-18 Change function and mechanism of rotation.
/// 2018-05-24 Add object cycle implementation for Initiate() calling.
/// 2018-05-25 Add object cycle for Start() calling.
/// 2018-07-02 Refactoring glm::vec3 to DVector3
/// 2018-07-15 Refactoring, and rename Instantiate to CreateGameObject
/// 2018-07-31 Add AddOffset... series function.
///

#include <algorithm>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

/// ::opgs16::element::CScriptFrame
#include <Component/script_frame.h>
#include <Component/particle_spawner.h>
#include <Component/Internal/component.h>
#include <Component/Internal/component_type.h>
/// import logger debug mode
#include <Headers/import_logger.h>
/// Type checking template
#include <Helper/template.h>
#include <Helper/Type/axis.h>
#include <Helper/Type/vector3.h>

/// Enforced assert macroes.
#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/activated.h>
/// Forward declaration
#include <opgs16fwd.h>
#include "Component/Internal/xyz_axis_renderer.h"


//!
//! Forward declaration
//!

namespace opgs16::component::_internal {
class CColliderBase;
class CPrivateXyzAxisRenderer;
}

namespace opgs16::element::_internal {
enum class EColliderCollisionState;
}

namespace opgs16::element {
class UiObject;
}

//!
//! Implementation
//!

namespace opgs16::element {

///
/// @class CObject
/// @brief The class for every object to update and draw.
///
/// Object abstract class stores common values and inheritable by derived each object class.
/// When using each m_object_list you have to use this type as a storing type to avoid cracking of
/// polymorphism.
/// Each object can be called using Update and Draw with shader to use.
///
class CObject {
  using TComponentSmtPtr  = std::unique_ptr<component::_internal::CComponent>;
  using TComponentPair    = std::pair<TComponentSmtPtr, component::_internal::EComponentType>;
  using TComponentList    = std::vector<TComponentPair>;
  using TNameCounterMap   = std::unordered_map<std::string, int32_t>;
	using TGameObjectSmtPtr = std::unique_ptr<CObject>;
	using TGameObjectMap    = std::unordered_map<std::string, TGameObjectSmtPtr>;
  using TPimplSmtPtr      = std::unique_ptr<_internal::CObjectImpl>;

public:
	CObject();
	virtual ~CObject();

  CObject(const CObject&) = delete;
  CObject& operator=(const CObject&) = delete;
  CObject(CObject&&) = default;
  CObject& operator=(CObject&&) = default;

  ///
  /// @brief
  /// Update components of object.
  ///
  /// @param[in] delta_time Elapsed delta time of this update frame.
  ///
  void Update(float delta_time);

  void Draw() { Render(); }

  ///
  /// @brief Return local position.
  /// @return Object's local position.
  ///
  const DVector3& GetLocalPosition() const noexcept;

	///
	/// @brief Return world position.
	/// @return Object's world position from parent object's position.
	///
	const DVector3& GetWorldPosition() const noexcept;

  ///
  /// @brief Get Object's final position.
  ///
  const DVector3& GetFinalPosition() const noexcept;

  ///
  /// @brief Sets local position.
  /// @param[in] position local position Position to set on.
  ///
  void SetLocalPosition(const DVector3& position) noexcept;

	///
	/// @brief Set world position.
	/// @param[in] world_position Winal position in Screen space and from parent' object.
	///
	void SetWorldPosition(const DVector3& world_position);

  ///
  /// @brief The method set final position but keeping parent position
  /// but adjusting CObject's world position.
  ///
  void SetWorldPositionWithFinalPosition(const DVector3& final_position);

  ///
  /// @brief Add offset value with axis as local position.
  ///
  void AddOffsetLocalPosition(EAxis3D axis, float value) noexcept;

  ///
  /// @brief Add offset value with axis as world position.
  ///
  void AddOffsetWorldPosition(EAxis3D axis, float value) noexcept;

  ///
  /// @brief The method gets local rotation angle value. (euler degree value)
  /// @return Object's rotation angle value.
  ///
  float GetLocalRotationAngle(EAxis3D direction) const noexcept;

  ///
  /// @brief The method gets world rotation angle value. (euler degree value)
  ///
  float GetWorldRotationAngle(EAxis3D direction) const noexcept;

  ///
  /// @brief The method gets final rendering angle value. (euler degree value)
  ///
  float GetFinalRotationAngle(EAxis3D direction) const noexcept;

  ///
  /// @brief The method adds local rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  /// @param[in] direction Direction axis.
  /// @param[in] angle_value Euler angle value to set on.
  ///
  void SetRotationLocalAngle(EAxis3D direction, float angle_value) noexcept;

  ///
  /// @brief The method adds world rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  /// @param[in] direction Direction axis.
  /// @param[in] angle_value Euler angle value to set on.
  ///
  void SetRotationWorldAngle(EAxis3D direction, float angle_value) noexcept;

  ///
  /// @brief Add offset value with axis as local rotation angle.
  ///
  void AddOffsetLocalAngle(EAxis3D axis, float value) noexcept;

  ///
  /// @brief Add offset value with axis as world rotation angle.
  ///
  void AddOffsetWorldAngle(EAxis3D axis, float value) noexcept;

  ///
  /// @brief The method gets (x, y, z) DVector3 scaling axis factor.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  ///
  const DVector3& GetLocalScale() const noexcept;

  ///
  /// @brief Returns world scale which able to affect child object's transform.
  ///
  const DVector3& GetWorldScale() const noexcept;

  ///
  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  ///
  void SetLocalScale(const DVector3& xyz_value) noexcept;

  ///
  /// @brief Sets world scaling vector value which have (x, y, z).
  ///
  void SetWorldScale(const DVector3& xyz_value) noexcept;

  ///
  /// @brief The method returns Model matrix, M = TRS
  ///
  /// If any value which related to Translate, Rotation, Scaling has been changed,
  /// model matrix would be calculated newly and returned.
  ///
  /// @return Model matrix (M = TRS)
  ///
  const glm::mat4& GetModelMatrix() const;

  ///
  /// @brief Set active option of object.
  /// If m_active is false, this object cannot update until m_active return to true.
  ///
  /// @param[in] value Active option value.
  ///
  void SetObjectActive(const bool value);

  phitos::enums::EActivated IsObjectInternallyActive() const;

  ///
  /// @brief
  ///
  phitos::enums::EActivated IsObjectActive() const;

  ///
  /// @brief Set tag with tag name. This method will check whether or not exist matched tag name
  /// in SettingManager. If not exist, do nothing and chagne error flag.
  /// @param[in] tag_name Tag name
  ///
  void SetObjectTag(const std::string& tag_name);

  ///
  /// @brief Overloaded version of SetObjectTag(tag_name)
  ///
  void SetObjectTag(const unsigned tag_index);

  ///
  /// @brief Get tag index of this object.
  /// @return Tag index value.
  ///
  unsigned GetObjectTagIndexOf() const;

  ///
  /// @brief Get Tag Name of this object. (different with name of object)
  /// This methods return tag name by referencing SettingManager in body.
  /// @return Tag name string.
  ///
  std::string GetObjectTagNameOf() const;

  ///
  /// @brief Return object name
  ///
  const std::string& GetGameObjectName() const noexcept {
    return m_object_name;
  }

  //!
  //! Object creation.
  //!

  ///
  /// @brief Return object final position not included local position.
  ///
  const DVector3& GetParentPosition() const noexcept;

	///
	/// @brief Destroy child object has unique tag key but not recursively.
	/// @param[in] child_object_name Object name.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroied, return ture.
	///
	bool DestroyGameObject(const std::string& child_object_name);

  ///
  /// @brief Destory child object with address.
  /// @param[in] child_object Object reference.
  /// @param[in] is_recursive Flag for destruction of specified object recursively.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroyed, return ture.
  ///
  bool DestroyGameObject(const CObject& child_object, bool is_recursive = false);

	///
	/// @brief Get children tag list.
	/// @return Children's tags container of object.
	///
	std::vector<std::string> GetGameObjectNameList() const;

	///
	/// @brief Get children reference.
	/// @return Children m_object_list component list.
	///
	TGameObjectMap& GetGameObjectList();

	///
	/// @brief Get arbitary child object.
	/// @param[in] object_name The name of object to find.
  /// @param[in] is_resursive
	/// @return Object's raw-pointer instance. this cannot removeable.
	///
	CObject* GetGameObject(const std::string& object_name, bool is_resursive = false);

	///
	/// @brief This initiate object as a child of base object.
	///
	/// This method create & initiate object as a child, make a leaf of object hierarchy tree.
	/// Created type instance is implicitly converted (up-cast) to Object class type.
	///
	/// If you must initiate specific derived class with initialization parameters,
	/// you can type them _args parameter. This method gets variadic inputs initialized with
	/// appropriate c-tor.
	///
	/// You have to <> parenthesis to input specific class type to create.
	///
  template <
    class TCObjectType,
    class... TConstructionArgs,
    class = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string object_name, TConstructionArgs&&... args) {
      const auto object_final_name = pCreateChildTag(object_name);

    auto [result_pair, result] = m_children_objects.try_emplace(object_final_name, nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    // Set hash value and parent as this.
    result_pair->second = std::make_unique<TCObjectType>(std::forward<TConstructionArgs>(args)...);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->pSetHash(object_final_name);
    object_ref->m_parent = this;

    return static_cast<TCObjectType*>(object_ref.get());
  }

  //!
  //! Component creation.
  //!

  ///
  /// @brief
  /// Add component and bind to this object instance.
  ///
  /// @param[in] TComponent Component type class argument.
  /// @param[in] _Params&& Universal reference. Used to be arguments of Component constructor.
  ///
  using _Component = component::_internal::CComponent;
  template<
    class TComponent,
    typename... _Params,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  TComponent* AddComponent(_Params&&... params) {
    using EComponentType = component::_internal::EComponentType;

    auto type = EComponentType::Normal;
    if constexpr (std::is_base_of_v<component::CScriptFrame, TComponent>) {
      type = EComponentType::Script;
    }
    else if constexpr (std::is_base_of_v<component::CParticleSpawner, TComponent>) {
      type = EComponentType::Particle;
    }

    m_components.push_back(std::make_pair(std::make_unique<TComponent>(std::forward<_Params>(params)...), type));

    if constexpr (std::is_base_of_v<component::CScriptFrame, TComponent>) {
      static_cast<component::CScriptFrame*>(m_components.rbegin()->first.get())
          ->Initiate();
    }

    return GetComponent<TComponent>();
  }

  ///
  /// @brief Return component raw-pointer.
  /// @tparam TComponent Component type argument.
  /// @return If found, return TComponent* but not found, return nullptr.
  ///
  template<
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  TComponent* GetComponent() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    for (auto& [component, type] : m_components) {
      if (component->DoesTypeMatch(TComponent::__hash_val, TComponent::__string_literal))
        return static_cast<TComponent*>(component.get());
    }

    // If there is no component to find.
    return nullptr;
  }

  ///
  /// @brief Return component raw-pointer list.
  /// @tparam TComponent Component type argument.
  /// @return If found, return TComponent* list but not found, return nullptr.
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  std::vector<TComponent*> GetComponents() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<TComponent*> result_component_list{};

    for (auto& [component, item] : m_components) {
      if (component->DoesTypeMatch(TComponent::__hash_val, TComponent::__string_literal))
        result_component_list.push_back(static_cast<TComponent*>(component.get()));
    }

    // If there is no component to find.
    return result_component_list;
  }

  ///
  /// @brief Get non-owning components list from component list of object.
  /// All components's bound object reference are undefined, so you should control
  /// these components well not touching object reference like a GetBindObject().
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  std::vector<std::unique_ptr<TComponent>> pPopComponents() {
    using opgs16::component::_internal::EComponentType;
    if (m_components.empty()) return decltype(pPopComponents<TComponent>()){};

    std::vector<std::unique_ptr<TComponent>> result_list;

    // m_componentsからTTypeであるコンポネントに対して他のところに移す。
    auto it = --m_components.end();
    int32_t remove_back_count = 0;
    for (auto& [component, item] : m_components) {
      if (!component) continue;
      if (component->DoesTypeMatch(OP16_GET_HASH(TComponent), TComponent::__string_literal)) {
        result_list.push_back(std::unique_ptr<TComponent>(static_cast<TComponent*>(component.release())));
        ++remove_back_count;

        while (it->second == EComponentType::Particle && &(it->first) != &component) {
          --it;
        }
        if (&(it->first) == &component) continue;
        component.swap(it->first);
        // ReSharper disable CppAssignedValueIsNeverUsed
        item = it->second;
        // ReSharper restore CppAssignedValueIsNeverUsed
        --it;
      }
    }
    // 移動した分だけ真後ろから削除する。
    while (remove_back_count > 0) {
      m_components.pop_back();
      --remove_back_count;
    }
    return std::move(result_list);
  }

  ///
  /// @brief Remove component.
  /// @tparam TComponent Component type argument.
  /// @return If found, return true but otherwise false.
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  bool RemoveComponent() {
    using component::CParticleSpawner;
    using component::_internal::EComponentType;
    using manager::object::pMoveParticleSpawner;

    auto it = std::find_if(m_components.begin(), m_components.end(),
        [](const auto& item) {
          return item.first->DoesTypeMatch(OP16_GET_HASH(TComponent), TComponent::__string_literal);
        }
    );

    if (it != m_components.cend()) {
      if (it->second == EComponentType::Particle) {
        auto ptr = std::unique_ptr<CParticleSpawner>(static_cast<CParticleSpawner*>(it->first.release()));
        pMoveParticleSpawner(ptr);
      }
      else if (it->second == EComponentType::Script) {
        static_cast<component::CScriptFrame*>(it->first.get())->Destroy();
      }

      /// Too much execution time
      m_components.erase(it);
      return true;
    }
    return false;
  }

protected:
  /// Pointer implementation heap instance.
	TPimplSmtPtr   m_data = nullptr;
  /// The container stores child object.
	TGameObjectMap m_children_objects;
  /// CComponent list of thie object.
  TComponentList m_components;

private:
  ///
  /// @brief Return hash value of this object.
  ///
  uint32_t pGetHash() const;

  ///
  /// @brief Set hash value
  ///
  void pSetHash(const std::string& name) const {
    PHITOS_ASSERT(m_hash_initialized == false, "Hash value of object is already defined.");

    m_object_name = name;
    m_hash_value = static_cast<uint32_t>(std::hash<std::string>{}(name));
    m_hash_initialized = true;

    PUSH_LOG_INFO_EXT(
      "Create hash value for object, [Name : {0}] [Hash : {1:x}]",
      name,
      m_hash_value);
  }

  ///
  /// @brief Create child object name.
  /// @param[in] name
  /// @return
  ///
  std::string pCreateChildTag(const std::string& name) noexcept;

  CObject* pGetGameObjectResursively(const std::string& object_name) noexcept;

  //!
  //! Propagation
  //!

  ///
  /// @brief Get parent world's normal axis value (x, y, z) which been affected by world rotation.
  ///
  const std::array<DVector3, 3>& pGetParentWorldPropagateAxisValue() const noexcept;

  ///
  /// @brief
  ///
  const DVector3& pGetParentWorldSummedPositionValue() const noexcept;

  ///
  /// @brief Get all parent's summed world rotation euler angle values (x, y, z).
  ///
  const DVector3& pGetParentSummedWorldRotationAngle() const noexcept;

  ///
  /// @brief Get all parent's producted world scale values (x, y, z);
  ///
  const DVector3& pGetParentSummedProductedScaleValue() const noexcept;

  ///
  /// @brief Get rotation vector of total world angle from object tree root.
  ///
  const DVector3& pfGetRotationTotalWorldAngle();

  ///
  /// @brief
  ///
  void pSetWorldPropagatedPositionForcely(const DVector3& vector) noexcept;

  ///
  /// @brief
  ///
  void PropagateActivation(phitos::enums::EActivated value) noexcept;

  ///
  /// @brief Calculate and set up new final activation flag.
  ///
  void CalculateActivation();

  ///
  /// @brief
  ///
  void Propagate();

protected:
  /// Local update method for derived object.
  virtual void LocalUpdate();

  /// Render method for derived object.
  virtual void Render() {};

private:
  void pCallPhysicsCallback(_internal::EColliderCollisionState call_state,
                            bool is_collision_function,
                            component::_internal::CColliderBase* collider);

  const std::array<DVector3, 3>& pfGetObjectWorldSpaceAxis() const noexcept;

  /// this object name
  mutable std::string m_object_name;
  mutable bool m_is_transform_initiated = false;

  /// Parent object. if nullptr, this object has no parent and be on scene.
  CObject* m_parent = nullptr;

  /// Object name counter to avoid duplicated object name
  TNameCounterMap   m_name_counter;
  mutable uint32_t  m_hash_value = 0;
  mutable bool      m_hash_initialized = false;

  friend opgs16::element::CScene;
  friend opgs16::element::UiObject;
  friend opgs16::component::_internal::CColliderBase;
  friend opgs16::component::_internal::CPrivateXyzAxisRenderer;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H