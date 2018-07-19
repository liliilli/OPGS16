
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/object.cc
///
/// @brief
/// Definition file of ../Public/object.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-18 Change function and mechanism of rotation. and Add comments.
/// 2018-05-25
/// Move inline function Update() to .cc file.
/// Implement Script file Start().
///

/// Header file
#include <Element/object.h>
/// ::opgs16::element::_internal::CObjectImpl
#include <Element/Impl/object_impl.h>

/// ::opgs16::component::CScriptFrame
#include <Component/script_frame.h>
/// import logger in debug mode.
#include <Headers/import_logger.h>

#include <Manager/object_manager.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::element {

namespace {
using _internal::CObjectImpl;
} /*! unnamed namespace */

CObject::CObject() : m_data{ std::make_unique<CObjectImpl>() } {
  SetObjectActive(true);
}

void CObject::Update(float delta_time) {
  using phitos::enums::EActivated;
  using CScriptFrame = component::CScriptFrame;
  using EComponentType = component::_internal::EComponentType;
  using EScriptStarted = component::_internal::EScriptStarted;

  if (m_data) {
    switch (IsObjectActive()) {
    case EActivated::Disabled: {
      if (m_data->IsCallbackNotCalled()) {
        for (auto& [component, type] : m_components) {
          if (component->IsComponentActive() == false)
            continue;

          if (type == EComponentType::Script) {
            auto script = static_cast<CScriptFrame*>(component.get());
            script->OnDisabled();
          }
        }

        m_data->SetCallbackFlagToFalse();
      }
    } break;
    case EActivated::Activated: {
      LocalUpdate();

      for (auto& [component, type] : m_components) {
        if (component->IsComponentActive() == false)
          continue;

        // At first, check if component is script type (based on CScriptFrame)
        // and cast component to base script type.
        // If Start() function is not called, call and turn on the start flag
        // not to be callled over twice.
        if (type == EComponentType::Script) {
          auto script = static_cast<CScriptFrame*>(component.get());

          if (script->m_started == EScriptStarted::NotStarted) {
            script->Start();
            script->m_started = decltype(script->m_started)::Started;
          }

          if (m_data->IsCallbackNotCalled()) {
            script->OnEnabled();
            script->Start();
          }
        }

        component->Update(delta_time);
      }
      m_data->SetCallbackFlagToFalse();
    } break;
    }

    for (auto& child : m_children) {
      if (child.second)
        child.second->Update(delta_time);
    }
  }
}

const DVector3& CObject::GetLocalPosition() const noexcept {
  return m_data->GetLocalPosition();
}

const DVector3& CObject::GetWorldPosition() const noexcept {
  return m_data->GetWorldPosition();
}

const DVector3& CObject::GetParentPosition() const noexcept {
  return m_data->GetParentPosition();
}

const DVector3& CObject::GetFinalPosition() const noexcept {
  return m_data->GetFinalPosition();
}

// ReSharper disable CppMemberFunctionMayBeConst
void CObject::SetLocalPosition(const DVector3& position) noexcept {
  // ReSharper restore CppMemberFunctionMayBeConst
  m_data->SetLocalPosition(position);
}

void CObject::SetWorldPosition(const DVector3& world_position) {
  m_data->SetWorldPosition(world_position);
  PropagateParentPosition();
}

void CObject::SetParentPosition(const DVector3& parent_position) {
  m_data->SetParentPosition(parent_position);
  PropagateParentPosition();
}

void CObject::PropagateParentPosition() {
  for (auto& child : m_children) {
    auto& child_ptr = child.second;
    /// If object is not empty and activated and permits succeeding positioning.
    //using phitos::enums::EActivated;
    //child_ptr->IsObjectActive() == EActivated::Activated &&
    if (child_ptr && child_ptr->GetSucceedingPositionFlag())
      child_ptr->SetParentPosition(GetParentPosition());
  }
}

// Rotation functions.

const float CObject::GetRotationLocalAngle(_internal::EDirection direction) const noexcept {
    return m_data->GetRotationLocalAngle(direction);
}

const float CObject::GetRotationFromParentAngle(_internal::EDirection direction) const noexcept {
    return m_data->GetRotationFromParentAngle(direction);
}

const float CObject::GetRotationWorldAngle(_internal::EDirection direction) const noexcept {
    return m_data->GetRotationWorldAngle(direction);
}

const float CObject::GetRotationWpAngle(_internal::EDirection direction) const noexcept {
    return m_data->GetRotationWpAngle(direction);
}

void CObject::SetRotationLocalAngle(_internal::EDirection direction, const float angle_value) noexcept {
	m_data->SetRotationLocalAngle(direction, angle_value);
}

void CObject::SetRotationParentAngle(_internal::EDirection direction, const float angle_value) noexcept {
    m_data->SetRotationParentAngle(direction, angle_value);
    PropagateParentRotation();
}

void CObject::SetRotationWorldAngle(_internal::EDirection direction, const float angle_value) noexcept {
    m_data->SetRotationWorldAngle(direction, angle_value);
    PropagateParentRotation();
}


void CObject::PropagateParentRotation() {
  for (auto& child : m_children) {
    auto& child_ptr = child.second;
    /// If object is not empty and activated and permits succeeding positioning.
    using phitos::enums::EActivated;
    if (child_ptr &&
      child_ptr->IsObjectActive() == EActivated::Activated &&
      child_ptr->GetSucceedingRotationFlag()) {
      for (const auto& direction : _internal::k_direction_list)
        child_ptr->SetRotationParentAngle(direction, GetRotationWpAngle(direction));
    }
  }
}

// Scaling functions

const float CObject::GetScaleValue() const noexcept {
    return m_data->GetScaleLocalValue();
}

const DVector3& CObject::GetScaleFactor() const noexcept {
    return m_data->GetScaleLocalFactor();
}

void CObject::SetScaleValue(const float scale_value) {
	m_data->SetScaleLocalValue(scale_value);
}

void CObject::SetScaleFactor(const DVector3& factor) {
	m_data->SetScaleLocalFactor(factor);
}

const glm::mat4& CObject::GetModelMatrix() const {
  return m_data->GetModelMatrix();
}

void CObject::SetSucceedingPositionFlag(bool value) noexcept {
    m_data->SetSucceedingPositionFlag(value);
}

void CObject::SetSucceedingRotationFlag(bool value) noexcept {
    m_data->SetSucceedingRotationFlag(value);
}

void CObject::SetSucceedingScalingFlag(bool value) noexcept {
    m_data->SetSucceedingScalingFlag(value);
}

bool CObject::GetSucceedingPositionFlag() const noexcept {
  return m_data->GetSucceedingPositionFlag();
}

bool CObject::GetSucceedingRotationFlag() const noexcept {
  return m_data->GetSucceedingRotationFlag();
}

bool CObject::GetSucceedingScalingFlag() const noexcept {
  return m_data->GetSucceedingScalingFlag();
}

std::vector<std::string> CObject::GetGameObjectNameList() const {
	std::vector<std::string> list;
	list.reserve(m_children.size());

	for (const auto& object_pair : m_children) {
    list.emplace_back(object_pair.first);
	}

	return list;
}

CObject::TGameObjectMap& CObject::GetGameObjectList() {
	 return m_children;
}

CObject* CObject::GetGameObject(const std::string& object_name,
                                bool is_recursive) {
  if (!is_recursive) {
    if (const auto it = m_children.find(object_name);
        it != m_children.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return nullptr;
}

CObject* CObject::GetGameObjectResursively(const std::string& object_name) noexcept {
  for (auto& element : m_children) {
    const auto& object = element.second;
    // If object is empty, pass it.
    if (object) continue;

    if (object->GetGameObjectName() == object_name) {
      return object.get();
    }

    if (const auto object_pointer = object->GetGameObject(object_name, true)) {
      return object_pointer;
    }
  }

  return nullptr;
}


bool CObject::DestroyGameObject(const std::string& child_name) {
  if (const auto it = m_children.find(child_name); it == m_children.end()) {
    PUSH_LOG_ERROR_EXT("Could not destroy child object, {0}. [Name : {0}]", child_name);
    return false;
  }
  else {
    manager::object::Destroy(*(it->second.get()), this);
    return true;
  }
}

bool CObject::DestroyGameObject(const element::CObject& child_object) {
  using TObjectMap = std::unordered_map<std::string, TGameObjectSmtPtr>;
  using TItType = TObjectMap::iterator;

  manager::object::Destroy(child_object, this);
  return true;
}

void CObject::SetObjectActive(const bool value) {
	m_data->SetActive(value);
  m_data->CalculateActivation();
  this->Propagate();
}

void CObject::Propagate() {
  using phitos::enums::EActivated;

  EActivated flag = EActivated::Disabled;
  if (m_data->IsActive() == EActivated::Activated &&
      m_data->IsAnyParentActivated() == EActivated::Activated) {
    flag = EActivated::Activated;
  }

  auto& child_list = GetGameObjectList();
  for (auto& [name, ptr] : child_list) {
    ptr->PropagateActivation(flag);
    ptr->CalculateActivation();
    ptr->Propagate();
  }
}

phitos::enums::EActivated CObject::IsObjectInternallyActive() const {
  return m_data->IsActive();
}

phitos::enums::EActivated CObject::IsObjectActive() const {
  return m_data->IsFinallyActivated();
}

void CObject::PropagateActivation(phitos::enums::EActivated value) noexcept {
  m_data->PropagateActivation(value);
}

void CObject::CalculateActivation() {
  m_data->CalculateActivation();
}

void CObject::SetTag(const std::string& tag_name) {
  m_data->SetTag(tag_name);
}

void CObject::SetTag(const unsigned tag_index) {
  m_data->SetTag(tag_index);
}

unsigned CObject::GetTagIndexOf() const {
    return m_data->GetTagIndexOf();
}

std::string CObject::GetTagNameOf() const {
    return m_data->GetTagNameOf();
}

CObject::~CObject() {
  m_data.release();
  m_children.clear();

  for (auto& [element, type] : m_components) {
    using component::_internal::EComponentType;
    if (type == EComponentType::Script) {
      static_cast<component::CScriptFrame*>(element.get())->Destroy();
    }
  }
}

} /// ::opgs16::element namespace

