#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H
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
///

#include <algorithm>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
/// ::opgs16::component::_internal Component type
#include <Component/Internal/type.h>
/// ::opgs16::element::_internal::EDirection
#include <Element/Internal/direction_type.h>
/// ::opgs16::element::CScriptFrame
#include <Component/script_frame.h>

/// import logger debug mode
#include <Headers/import_logger.h>
/// Type checking template
#include <Helper/template.h>
/// Enforced assert macroes.
#include <Phitos/Dbg/assert.h>

/// Forward declaration
#include <opgs16fwd.h>
/// ::opgs16::DVector3
#include <Helper/Type/vector3.h>
#include "Phitos/Enums/activated.h"
#include <Phitos/Dbg/assert.h>

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
  using TComponentPair    = std::pair<TComponentSmtPtr,
                                      component::_internal::EComponentType>;
  using TComponentList    = std::vector<TComponentPair>;
  using TNameCounterMap   = std::unordered_map<std::string, int32_t>;
	using TGameObjectSmtPtr = std::unique_ptr<CObject>;
	using TGameObjectMap    = std::unordered_map<std::string, TGameObjectSmtPtr>;
  using TPimplSmtPtr      = std::unique_ptr<_internal::CObjectImpl>;

public:
	CObject();
	virtual ~CObject();

  ///
  /// @brief
  /// Update components of object.
  ///
  /// @param[in] delta_time Elapsed delta time of this update frame.
  ///
  void Update(float delta_time);

  void Draw() { Render(); }

  /// This method will be called when Collision.
  virtual void OnCollisionEnter(component::CRigidbody2D& collider) {};

  /// This method will be called when Trigger entered.
  virtual void OnTriggerEnter(component::CRigidbody2D& collider) {};

  ///
  /// @brief Return local position.
  /// @return Object's local position.
  ///
  const DVector3& GetLocalPosition() const noexcept;

  ///
  /// @brief Sets local position.
  /// @param[in] position local position Position to set on.
  ///
  void SetLocalPosition(const DVector3& position) noexcept;

	///
	/// @brief Return world position.
	/// @return Object's world position from parent object's position.
	///
	const DVector3& GetWorldPosition() const noexcept;

	///
	/// @brief Set world position.
	/// @param[in] world_position Winal position in Screen space and from parent' object.
	///
	void SetWorldPosition(const DVector3& world_position);

	///
	/// @brief The method refresh parent position.
	/// @param[in] parent_position Position to apply for.
	///
	void SetParentPosition(const DVector3& parent_position);

  ///
  /// @brief The method set final position but keeping parent position
  /// but adjusting CObject's world position.
  ///
  void SetWorldPosWithFinalPos(const DVector3& final_position);

  ///
  /// @brief Get Object's final position.
  ///
  const DVector3& GetFinalPosition() const noexcept;

  ///
  /// @brief The method gets rotation angle value
  /// @return Object's rotation angle value.
  ///
  const float GetRotationLocalAngle(_internal::EDirection direction) const noexcept;

  const float GetRotationFromParentAngle(_internal::EDirection direction) const noexcept;

  const float GetRotationWorldAngle(_internal::EDirection direction) const noexcept;

  const float GetRotationWpAngle(_internal::EDirection direction) const noexcept;

  ///
  /// @brief The method sets rotation angle values.
  /// When input value is positive and factor is [0, 1], axis rotates clockwise.
  /// input value is negative and factor is [0, 1], axis rotates counter-clockwise.
  /// @param[in] angle_value Angle value to set on.
  ///
  void SetRotationLocalAngle(_internal::EDirection direction, float angle_value) noexcept;

  void SetRotationParentAngle(_internal::EDirection direction, float angle_value) noexcept;

  void SetRotationWorldAngle(_internal::EDirection direction, float angle_value) noexcept;

  ///
  /// @brief The method gets scaling values
  /// @return Object's scaling value.
  ///
  const float GetScaleValue() const noexcept;

  ///
  /// @brief The method gets (x, y, z) DVector3 scaling axis factor.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  ///
  const DVector3& GetScaleFactor() const noexcept;

  ///
  /// @brief The method sets scaling angle values.
  /// @param[in] scale_value Scaling value to set on.
  ///
  void SetScaleValue(const float scale_value);

  ///
  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  /// @param[in] factor Scaling factor
  ///
  void SetScaleFactor(const DVector3& factor);

  ///
  /// @brief The method returns Model matrix, M = TRS
  ///
  /// If any value which related to Translate, Rotation, Scaling has been changed,
  /// model matrix would be calculated newly and returned.
  ///
  /// @return Model matrix (M = TRS)
  ///
  const glm::mat4& GetModelMatrix() const;

  void SetSucceedingPositionFlag(bool value) noexcept;

  void SetSucceedingRotationFlag(bool value) noexcept;

  void SetSucceedingScalingFlag(bool value) noexcept;

  bool GetSucceedingPositionFlag() const noexcept;

  bool GetSucceedingRotationFlag() const noexcept;

  bool GetSucceedingScalingFlag() const noexcept;

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
  /// @brief
  /// Overloaded function of Instantiate(Varadic...)
  ///
  template <
    class TCObjectType,
    class = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string& object_name,
                            std::unique_ptr<TCObjectType>& object_smtptr) {
    const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_children.try_emplace(
        object_final_name,
        nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::move(object_smtptr);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->SetHash(object_final_name);
    object_ref->SetParentPosition(GetParentPosition());
    return static_cast<TCObjectType*>(object_ref.get());
  }

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
  TCObjectType* CreateGameObject(const std::string object_name,
                            TConstructionArgs&&... args) {
      const auto object_final_name = CreateChildTag(object_name);

    auto [result_pair, result] = m_children.try_emplace(
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
	/// @brief Destroy child object has unique tag key but not recursively.
	/// @param[in] name Object name.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroied, return ture.
	///
	bool DestroyGameObject(const std::string& name);

  ///
  /// @brief Destory child object with address.
  /// @param[in] child_object Object reference.
  /// @param[in] is_recursive Flag for destruction of specified object recursively.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroyed, return ture.
  ///
  bool DestroyGameObject(const element::CObject& child_object,
                         bool is_recursive = false);

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
	/// @return Object's raw-pointer instance. this cannot removeable.
	///
	CObject* GetGameObject(const std::string& object_name,
                         bool is_resursive = false);

  ///
  /// @brief
  /// Add component and bind to this object instance.
  ///
  /// @param[in] _Ty Component type class argument.
  /// @param[in] _Params&& Universal reference. Used to be arguments of Component constructor.
  ///
  using _Component = component::_internal::CComponent;
  template<
    class _Ty,
    typename... _Params,
    typename = std::enable_if_t<std::is_base_of_v<_Component, _Ty>>
  >
  _Ty* AddComponent(_Params&&... params) {
    using EComponentType = component::_internal::EComponentType;

    auto type = EComponentType::Normal;
    if constexpr (std::is_base_of_v<component::CScriptFrame, _Ty>)
      type = EComponentType::Script;

    m_components.push_back(std::make_pair(
        std::make_unique<_Ty>(std::forward<_Params>(params)...), type)
    );

    if constexpr (std::is_base_of_v<component::CScriptFrame, _Ty>) {
      static_cast<component::CScriptFrame*>(m_components.rbegin()->first.get())
          ->Initiate();
    }

    return GetComponent<_Ty>();
  }

  ///
  /// @brief Return component raw-pointer.
  /// @tparam TTy Component type argument.
  /// @return If found, return _Ty* but not found, return nullptr.
  ///
  template<
    class TTy,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TTy>>
  >
  TTy* GetComponent() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    for (auto& [component, type] : m_components) {
      if (component->DoesTypeMatch(TTy::type, TTy::__string_literal))
        return static_cast<TTy*>(component.get());
    }

    // If there is no component to find.
    return nullptr;
  }

  ///
  /// @brief
  ///
  ///
  /// @param[in] TType
  ///
  /// @return
  ///
  ///
  template <
    class TType,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TType>>
  >
  std::vector<TType*> GetComponents() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<TType*> result_component_list{};

    for (auto& [component, item] : m_components) {
      if (component->DoesTypeMatch(TType::type, TType::__string_literal))
        result_component_list.push_back(static_cast<TType*>(component.get()));
    }

    // If there is no component to find.
    return result_component_list;
  }

  ///
  /// @brief Remove component.
  /// @param[in] _Ty Component type argument.
  /// @return If found, return true but otherwise false.
  ///
  template <
    class _Ty,
    typename = std::enable_if_t<std::is_base_of_v<_Component, _Ty>>
  >
  bool RemoveComponent() {
    auto it = std::find_if(
        m_components.cbegin(),
        m_components.cend(),
        [](const auto& item) {
          return item.first->DoesTypeMatch(_Ty::type, _Ty::__string_literal);
        }
    );

    if (it != m_components.cend()) {
      using EComponentType = component::_internal::EComponentType;
      if (it->second == EComponentType::Script) {
        static_cast<component::CScriptFrame*>(it->first.get())->Destroy();
        m_components.erase(it);    /*! Too much execution time */
      }
      return true;
    }
    else return false;
  }

  ///
  /// @brief Set tag with tag name. This method will check whether or not exist matched tag name
  /// in SettingManager. If not exist, do nothing and chagne error flag.
  /// @param[in] tag_name Tag name
  ///
  void SetTag(const std::string& tag_name);

  ///
  /// @brief Overloaded version of SetTag(tag_name)
  ///
  void SetTag(const unsigned tag_index);

  ///
  /// @brief Get tag index of this object.
  /// @return Tag index value.
  ///
  unsigned GetTagIndexOf() const;

  ///
  /// @brief Get Tag Name of this object. (different with name of object)
  /// This methods return tag name by referencing SettingManager in body.
  /// @return Tag name string.
  ///
  std::string GetTagNameOf() const;

  ///
  /// @brief Return hash value of this object.
  ///
  inline unsigned GetHash() const {
    return m_hash_value;
  }

  ///
  /// @brief Set hash value
  ///
  inline void SetHash(const std::string& name) const {
    PHITOS_ASSERT(m_hash_initialized == false,
      "Hash value of object is already defined.");

    if (!m_hash_initialized) {
      m_object_name = name;
      m_hash_value = static_cast<int32_t>(std::hash<std::string>{}(name));
      m_hash_initialized = true;

      PUSH_LOG_INFO_EXT(
        "Create hash value for object, [Name : {0}] [Hash : {1:x}]",
        name,
        m_hash_value);
    }
  }

  ///
  /// @brief Return object name
  ///
  inline const std::string& GetGameObjectName() const {
    return m_object_name;
  }

  ///
  /// @brief Return object final position not included local position.
  ///
  const DVector3& GetParentPosition() const noexcept;

protected:
	TPimplSmtPtr   m_data{ nullptr }; /*! Pointer implementation heap instance. */
	TGameObjectMap  m_children;        /*! The container stores child object. */
  TComponentList m_components{}; /*! CComponent list of thie object. */

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

  /// Propagate parent position recursively.
  void PropagateParentPosition();

  /// Propagate parent rotation recursively.
  void PropagateParentRotation();

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
  void Propagate() ;

protected:
  /// Local update method for derived object.
  virtual void LocalUpdate() {};

  /// Render method for derived object.
  virtual void Render() {};

private:
  /// Object name counter to avoid duplicated object name
  TNameCounterMap m_name_counter;

  /// this object name
  mutable std::string m_object_name;
  /// Hash value to verify object number
  mutable int32_t m_hash_value = 0;
  /// Flag
  mutable bool m_hash_initialized = false;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H