#include <precompiled.h>
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
/// 2018-07-31 Add AddOffset... series function.
///

/// Header file
#include <Element/object.h>

#include <Phitos/Dbg/assert.h>

/// ::opgs16::component::CScriptFrame
#include <Component/script_frame.h>
#include <Component/particle_spawner.h>
#include <Element/Internal/physics_enums.h>
#include <Element/Impl/object_impl.h>
#include <Headers/import_logger.h>
#include <Manager/object_manager.h>

#include <Core/core_setting.h>

namespace opgs16::element {

namespace {
using _internal::CObjectImpl;
} /*! unnamed namespace */

CObject::CObject() : m_data{ std::make_unique<CObjectImpl>() } {
  using opgs16::setting::IsEnableDebugMode;
  using opgs16::component::_internal::CPrivateXyzAxisRenderer;
  SetObjectActive(true);

  if (IsEnableDebugMode()) {
    AddComponent<CPrivateXyzAxisRenderer>(*this);
  }
}

void CObject::Update(float delta_time) {
  using phitos::enums::EActivated;
  using CScriptFrame   = component::CScriptFrame;
  using EComponentType = component::_internal::EComponentType;
  using EScriptStarted = component::_internal::EScriptStarted;

  if (!m_data) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  switch (IsObjectActive()) {
  case EActivated::Disabled: {
    if (m_data->IsCallbackNotCalled()) {
      for (auto& [component, type] : m_components) {
        if (!(component && component->IsComponentActive()))
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
    if (!m_is_transform_initiated) {
      if (!m_parent) {
        m_is_transform_initiated = true;
      }
      else {
        PUSH_LOG_DEBUG_EXT("Aligned : {} from {}.", m_object_name, m_parent->GetGameObjectName());
        m_is_transform_initiated = true;
      }
    }
    if (m_parent) {
      // Realign transform from parent.
      m_data->SetObjectWorldAxisBasisValue(m_parent->pGetParentWorldPropagateAxisValue());
      m_data->SetObjectWorldRotationBasisValue(m_parent->pGetParentSummedWorldRotationAngle());
      //m_data->SetObjectWorldScaleBasisValue(m_parent->pGetParentProductedWorldScaleValue());
    }

    for (auto& [component, type] : m_components) {
      if (!(component && component->IsComponentActive())) continue;

      // At first, check if component is script type (based on CScriptFrame) and cast component to base script type.
      // If Start() function is not called, call and turn on the start flag not to be callled over twice.
      if (type == EComponentType::Script) {
        auto script = static_cast<CScriptFrame*>(component.get());
        if (script->m_started == EScriptStarted::NotStarted) {
          script->Start();
          script->m_started = decltype(script->m_started)::Started;
        }
        if (m_data->IsCallbackNotCalled()) {
          script->OnEnabled();
        }
      }

      component->Update(delta_time);
    }

    LocalUpdate();
    m_data->SetCallbackFlagToFalse();

    // Update children objects.
    for (auto& child : m_children_objects) {
      if (child.second)
        child.second->Update(delta_time);
    }
  } break;
  }
}

const std::array<DVector3, 3>& CObject::pGetParentWorldPropagateAxisValue() const noexcept {
  return m_data->GetChildObjectWorldAxisBasisValue();
}

const DVector3& CObject::pGetParentSummedWorldRotationAngle() const noexcept {
  return m_data->GetWorldSummedRotationAngle();
}

const DVector3& CObject::pGetParentProductedWorldScaleValue() const noexcept {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return m_data->GetLocalScale();
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

void CObject::SetLocalPosition(const DVector3& position) noexcept {
  m_data->SetLocalPosition(position);
  m_data->GetFinalPosition();
}

void CObject::SetWorldPosition(const DVector3& world_position) {
  m_data->SetWorldPosition(world_position);
  PropagateParentPosition();
  m_data->GetFinalPosition();
}

void CObject::AddOffsetLocalPosition(EAxis3D axis, float value) noexcept {
  m_data->AddOffsetLocalPosition(axis, value);
  //PropagateParentPosition();
  m_data->GetFinalPosition();
}

void CObject::AddOffsetWorldPosition(EAxis3D axis, float value) noexcept {
  m_data->AddOffsetWorldPosition(axis, value);
  PropagateParentPosition();
  m_data->GetFinalPosition();
}

void CObject::SetParentPosition(const DVector3& parent_position) {
  m_data->SetParentPosition(parent_position);
  PropagateParentPosition();
}

void CObject::SetWorldPosWithFinalPos(const DVector3& final_position) {
  m_data->SetWorldPosWithFinalPos(final_position);
  PropagateParentPosition();
}

void CObject::PropagateParentPosition() {
  for (auto& [object_name, object] : m_children_objects) {
    if (!object) continue;
    object->SetParentPosition(GetParentPosition());
  }
}

// Rotation functions.

float CObject::GetRotationLocalAngle(EAxis3D direction) const noexcept {
  return m_data->GetLocalRotationAngle(direction);
}

float CObject::GetRotationWorldAngle(EAxis3D direction) const noexcept {
  return m_data->GetWorldRotationAngle(direction);
}

float CObject::GetRotationWpAngle(EAxis3D direction) const noexcept {
  return m_data->GetRotationWpAngle(direction);
}

void CObject::SetRotationLocalAngle(EAxis3D direction, const float angle_value) noexcept {
	m_data->SetLocalRotationAngle(direction, angle_value);
}

void CObject::SetRotationParentAngle(EAxis3D direction, const float angle_value) noexcept {
  m_data->SetRotationParentAngle(direction, angle_value);
  PropagateParentRotation();
}

void CObject::AddOffsetLocalAngle(EAxis3D axis, float value) noexcept {
  m_data->AddOffsetLocalAngle(axis, value);
}

void CObject::AddOffsetWorldAngle(EAxis3D axis, float value) noexcept {
  m_data->AddOffsetWorldAngle(axis, value);
  PropagateParentRotation();
}

void CObject::SetRotationWorldAngle(EAxis3D direction, const float angle_value) noexcept {
  m_data->SetWorldRotationAngle(direction, angle_value);
  PropagateParentRotation();
}


void CObject::PropagateParentRotation() {
  for (auto& child : m_children_objects) {
    auto& child_ptr = child.second;
    /// If object is not empty and activated and permits succeeding positioning.
    using phitos::enums::EActivated;
    if (child_ptr &&
      child_ptr->IsObjectActive() == EActivated::Activated) {
      child_ptr->SetRotationParentAngle(EAxis3D::X, GetRotationWpAngle(EAxis3D::X));
      child_ptr->SetRotationParentAngle(EAxis3D::Y, GetRotationWpAngle(EAxis3D::Y));
      child_ptr->SetRotationParentAngle(EAxis3D::Z, GetRotationWpAngle(EAxis3D::Z));
    }
  }
}

// Scaling functions

const DVector3& CObject::GetLocalScale() const noexcept {
  return m_data->GetLocalScale();
}

const DVector3& CObject::GetWorldScale() const noexcept {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return {};
}

void CObject::SetLocalScale(const DVector3& factor) {
	m_data->SetLocalScale(factor);
}

void CObject::SetWorldScale(const DVector3& xyz_value) {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

const glm::mat4& CObject::GetModelMatrix() const {
  return m_data->GetModelMatrix();
}

std::vector<std::string> CObject::GetGameObjectNameList() const {
	std::vector<std::string> list;
	list.reserve(m_children_objects.size());

	for (const auto& object_pair : m_children_objects) {
    list.emplace_back(object_pair.first);
	}

	return list;
}

CObject::TGameObjectMap& CObject::GetGameObjectList() {
	 return m_children_objects;
}

CObject* CObject::GetGameObject(const std::string& object_name, bool is_recursive) {
  if (!is_recursive) {
    if (const auto it = m_children_objects.find(object_name); it != m_children_objects.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return nullptr;
}

CObject* CObject::pGetGameObjectResursively(const std::string& object_name) noexcept {
  for (auto& element : m_children_objects) {
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
  if (const auto it = m_children_objects.find(child_name); it == m_children_objects.end()) {
    PUSH_LOG_ERROR_EXT("Could not destroy child object, {0}. [Name : {0}]", child_name);
    return false;
  }
  else {
    const auto result = manager::object::DestroyGameObject(*(it->second.get()), this, false);
    return result;
  }
}

bool CObject::DestroyGameObject(const element::CObject& child_object,
                                bool is_recursive) {
  const auto result = manager::object::DestroyGameObject(
      child_object, this, is_recursive);
  return result;
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

void CObject::pCallPhysicsCallback(_internal::EColliderCollisionState call_state,
                                   bool is_collision_function,
                                   component::_internal::CColliderBase* collider) {
  using opgs16::component::_internal::EComponentType;
  using opgs16::component::CScriptFrame;
  std::vector<CScriptFrame*> m_script_frame;

  // @todo 外に抜いて一々作らないようにすること。
  for (auto& [component_smtptr, component_type] : m_components) {
    if (component_smtptr && component_type == EComponentType::Script) {
      m_script_frame.push_back(static_cast<CScriptFrame*>(component_smtptr.get()));
    }
  }

  switch (call_state) {
  case _internal::EColliderCollisionState::Idle:  // Exit
    for (auto& script : m_script_frame) {
      if (is_collision_function) script->OnCollisionExit(collider);
      else script->OnTriggerExit(collider);
    }
    break;
  case _internal::EColliderCollisionState::Enter: // Enter
    for (auto& script : m_script_frame) {
      if (is_collision_function) script->OnCollisionEnter(collider);
      else script->OnTriggerEnter(collider);
    }
    break;
  case _internal::EColliderCollisionState::Stay:  // Stay
    for (auto& script : m_script_frame) {
      if (is_collision_function) script->OnCollisionStay(collider);
      else script->OnTriggerStay(collider);
    }
    break;
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

const DVector3& CObject::pfGetRotationTotalWorldAngle() {
  return m_data->GetWorldSummedRotationAngle();
}

void CObject::CalculateActivation() {
  m_data->CalculateActivation();
}

void CObject::SetObjectTag(const std::string& tag_name) {
  m_data->SetTag(tag_name);
}

void CObject::SetObjectTag(const unsigned tag_index) {
  m_data->SetTag(tag_index);
}

unsigned CObject::GetObjectTagIndexOf() const {
  return m_data->GetTagIndexOf();
}

std::string CObject::GetObjectTagNameOf() const {
  return m_data->GetTagNameOf();
}

CObject::~CObject() {
  using opgs16::component::CParticleSpawner;
  using opgs16::manager::object::pMoveParticleSpawner;

  m_data.release();
  m_children_objects.clear();

  for (auto& [element, type] : m_components) {
    using component::_internal::EComponentType;
    if (type == EComponentType::Script) {
      static_cast<component::CScriptFrame*>(element.get())->Destroy();
    }
  }

  auto spawner_list = pPopComponents<component::CParticleSpawner>();
  for (auto& spawner : spawner_list) {
    pMoveParticleSpawner(spawner);
  }
}

uint32_t CObject::pGetHash() const {
  return m_hash_value;
}

std::string CObject::pCreateChildTag(const std::string& name) noexcept {
  if (m_name_counter.find(name) != m_name_counter.end()) {
    auto& count = m_name_counter[name];
    count += 1;
    return fmt::format("{}_{}", name, count);
  }

  m_name_counter[name] = 0;
  return name;
}

} /// ::opgs16::element namespace

