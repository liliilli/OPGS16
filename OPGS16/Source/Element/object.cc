
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

namespace opgs16::element {

namespace {
using _internal::CObjectImpl;
} /*! unnamed namespace */

CObject::CObject() : m_data{ std::make_unique<CObjectImpl>() } { }

void CObject::Update(float delta_time) {
  if (m_data && GetActive()) {
    LocalUpdate();

    for (auto&[component, type] : m_components) {
      using CScriptFrame    = component::CScriptFrame;
      using EComponentType  = component::_internal::EComponentType;
      using EScriptStarted  = component::_internal::EScriptStarted;

      // At first, check if component is script type (based on CScriptFrame)
      // and cast component to base script type.
      // If Start() function is not called, call and turn on the start flag
      // not to be callled over twice.
      if (type == EComponentType::Script) {
        if (auto script = static_cast<CScriptFrame*>(component.get());
            script->m_started == EScriptStarted::NotStarted) {

          PUSH_LOG_INFO_EXT(
              "Object call Start() : [Name : {0}]",
              script->GetBindObject().GetObjectName());
          script->Start();
          script->m_started = decltype(script->m_started)::Started;
        }
      }

      component->Update(delta_time);
    }

    for (auto& child : m_children) {
      // If child.second is not emtpy and activated.
      if (child.second && child.second->GetActive())
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
    /*! If object is not empty and activated and permits succeeding positioning. */
    if (child_ptr && child_ptr->GetActive() && child_ptr->GetSucceedingPositionFlag())
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
        /*! If object is not empty and activated and permits succeeding positioning. */
        if (child_ptr && child_ptr->GetActive() && child_ptr->GetSucceedingRotationFlag()) {
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

std::vector<std::string> CObject::GetChildrenNameList() const {
	std::vector<std::string> list;
	list.resize(m_children.size());

	for (const auto& object_pair : m_children) {
        /*! emplace_back evades unnecessary temp instance. */
	    list.emplace_back(object_pair.first);
	}

	return list;
}

CObject::object_map& CObject::GetChildList() {
	 return m_children;
 }

CObject::object_raw const CObject::GetChild(const std::string& tag) {
    if (m_children.find(tag) == m_children.end()) return nullptr;
    return m_children.at(tag).get();
}

bool CObject::DestroyChild(const std::string& child_name) {
  if (const auto it = m_children.find(child_name); it == m_children.end()) {
    PUSH_LOG_ERROR_EXT("Could not destroy child object, {0}. [Name : {0}]", child_name);
    return false;
  }
  else {
    manager::object::Destroy(*(it->second.get()));
    return true;
  }
}

bool CObject::GetActive() const {
    return m_data->GetActive();
}

void CObject::SetActive(const bool value) {
	m_data->SetActive(value);
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

CObject::~CObject() = default;

} /// ::opgs16::element namespace

