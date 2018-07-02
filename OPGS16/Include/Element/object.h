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
///
/// @log
/// 2018-02-19
/// Refactoring. Remove Draw(ShaderNew) obsolete not helpful method. Yeah!
/// 2018-02-23
/// Add hierarchy rotation and scaling option. (mechanism is not implemented yet)
/// 2018-03-05
/// Add member function related to controlling rendering layer.
/// 2018-03-11 Move contents into ::opgs16::element namespace.
/// 2018-04-16 Add rotation (parent, world) get/set functions.
/// 2018-04-18 Change function and mechanism of rotation.
/// 2018-05-24 Add object cycle implementation for Initiate() calling.
/// 2018-07-02 Refactoring glm::vec3 to DVector3
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

/// import logger debug mode
#include <Headers/import_logger.h>
/// Type checking template
#include <Helper/template.h>
/// Enforced assert macroes.
#include <Helper/assert.h>

/// Forward declaration
#include <opgs16fwd.h>
/// ::opgs16::DVector3
#include <Helper/Type/vector3.h>

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
/// @log
/// 2018-02-19 Refactoring. Remove Draw(ShaderNew) obsolete not helpful method. Yeah!
/// 2018-02-19
/// Add GetParentPosition(). Remove virtual property from Draw() and Render()
/// virtual methods. and Remove virtual property from Update() instead of
/// adding LocalUpdate() method which replaces Update() override.
/// 2018-03-05 Add member function related to controlling rendering layer.
/// 2018-03-11 Move contents into ::opgs16::element namespace.
/// 2018-04-16 Add rotation (parent, world) get/set functions.
/// 2018-04-18 Change function and mechanism of rotation.
/// 2018-05-24 Add object cycle implementation for Initiate() calling.
/// 2018-05-25 Add object cycle for Start() calling.
/// 2018-07-02 Refactoring glm::vec3 to DVector3
///
class CObject {
  using component_ptr     = std::unique_ptr<component::_internal::CComponent>;
  using component_list    = std::vector<
      std::pair<component_ptr, component::_internal::EComponentType>>;
  using name_counter_map  = std::unordered_map<std::string, unsigned>;
	using object_raw = CObject*;
	using object_ptr = std::unique_ptr<CObject>;
	using object_map = std::unordered_map<std::string, object_ptr>;
  using pimpl_ptr  = std::unique_ptr<_internal::CObjectImpl>;

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
  /// @param[in] value Active option value.
  ///
  void SetActive(const bool value);

  bool GetActive() const;

  ///
  /// @brief
  /// Overloaded function of Instantiate(Varadic...)
  ///
  template <
    class _Ty,
    class = std::enable_if_t<IsCObjectBase<_Ty>>
  >
  _Ty* Instantiate(const std::string name, std::unique_ptr<_Ty>& instance) {
    const auto item_tag = CreateChildTag(name);
    m_children[item_tag] = std::move(instance);
    m_children[item_tag]->SetHash(item_tag);
    m_children[item_tag]->SetParentPosition(GetParentPosition());

    auto list = m_children[item_tag]->GetComponents<component::CScriptFrame>();

    unsigned index = 0;
    for (const auto script_ptr : list) {
      script_ptr->Initiate();
      PUSH_LOG_INFO_EXT(
          "Object call Initiate() : [Name : {0}] [Id : {1}]",
          item_tag, index);
      ++index;
    }

    return static_cast<_Ty*>(m_children[item_tag].get());
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
	/// @param[in] object Object instance to make.
	/// @param[in] name Object Tag.
	/// @tparam _Args variadic args to be used c-tor initialize parameters inputs.
	/// @return Success/Failed flag. If the methods success to make child object, return true.
	///
  template <
    class _Ty,
    class... _Args,
    class = std::enable_if_t<IsCObjectBase<_Ty>>
  >
  _Ty* Instantiate(const std::string name, _Args&&... _args) {
    const auto item_tag = CreateChildTag(name);
    m_children.emplace(item_tag, std::make_unique<_Ty>(std::forward<_Args>(_args)...));
    m_children[item_tag]->SetHash(item_tag);
    m_children[item_tag]->SetParentPosition(GetParentPosition());

    auto list = m_children[item_tag]->GetComponents<component::CScriptFrame>();
    unsigned index = 0;
    for (const auto script_ptr : list) {
      script_ptr->Initiate();
      PUSH_LOG_INFO_EXT(
          "Object call Initiate() : [Name : {0}] [Id : {1}]",
          item_tag, index);
      ++index;
    }

    return static_cast<_Ty*>(m_children[item_tag].get());
  }

	///
	/// @brief Destroy child object has unique tag key.
	/// @param[in] name Object name.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroied, return ture.
	///
	bool DestroyChild(const std::string& name);

	///
	/// @brief Get children tag list.
	/// @return Children's tags container of object.
	///
	std::vector<std::string> GetChildrenNameList() const;

	///
	/// @brief Get children reference.
	/// @return Children m_object_list component list.
	///
	object_map& GetChildList();

	///
	/// @brief Get arbitary child object.
	/// @param[in] child_name The name of object to find.
	/// @return Object's raw-pointer instance. this cannot removeable.
	///
	object_raw const GetChild(const std::string& child_name);

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
        std::make_unique<_Ty>(std::forward<_Params>(params)...),
        type)
    );

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
      if (component->DoesTypeMatch(TTy::type))
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
      if (component->DoesTypeMatch(TType::type))
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
      [](const std::unique_ptr<component::Component>& item) {
      return item->DoesTypeMatch(_Ty::type);
    }
    );

    if (it != m_components.cend()) {
      m_components.erase(it);    /*! Too much execution time */
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
    NEU_ASSERT(m_hash_initialized == false,
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
  inline const std::string& GetObjectName() const {
    return m_object_name;
  }

  ///
  /// @brief Return object final position not included local position.
  ///
  const DVector3& GetParentPosition() const noexcept;

private:
  /// Object name counter to avoid duplicated object name
  name_counter_map m_name_counter;

  /// this object name
  mutable std::string m_object_name{};
  /// Hash value to verify object number
  mutable int32_t m_hash_value{};
  /// Flag
  mutable bool m_hash_initialized{ false };

protected:
	pimpl_ptr   m_data{ nullptr }; /*! Pointer implementation heap instance. */
	object_map  m_children;        /*! The container stores child object. */
  component_list m_components{}; /*! CComponent list of thie object. */

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

  /// Propagate parent position recursively.
  void PropagateParentPosition();

  /// Propagate parent rotation recursively.
  void PropagateParentRotation();

protected:
  /// Local update method for derived object.
  virtual void LocalUpdate() {};

  /// Render method for derived object.
  virtual void Render() {};
};

} /// ::opgs16::element namespace

#endif /// OPGS16_SYSTEM_ELEMENT_PUBLIC_OBJECT_H