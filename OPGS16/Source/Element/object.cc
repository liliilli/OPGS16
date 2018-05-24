/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Private/object.cc
 * @brief Definition file of ../Public/object.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-04-18 Change function and mechanism of rotation. and Add comments.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\object.h>             /// Header file
/// ::opgs16::element::_internal::CObjectImpl
#include <Element\Impl\object_impl.h>   

namespace opgs16 {
namespace element {

namespace {
using _internal::CObjectImpl;
} /*! unnamed namespace */

CObject::CObject() : m_data{ std::make_unique<CObjectImpl>() } { }

const glm::vec3& CObject::GetLocalPosition() const noexcept {
    return m_data->GetLocalPosition();
}

const glm::vec3& CObject::GetWorldPosition() const noexcept {
    return m_data->GetWorldPosition();
}

const glm::vec3& CObject::GetParentPosition() const noexcept {
    return m_data->GetParentPosition();
}

const glm::vec3& CObject::GetFinalPosition() const noexcept {
    return m_data->GetFinalPosition();
}

// ReSharper disable CppMemberFunctionMayBeConst
void CObject::SetLocalPosition(const glm::vec3& position) noexcept {
    // ReSharper restore CppMemberFunctionMayBeConst
	m_data->SetLocalPosition(position);
}

void CObject::SetWorldPosition(const glm::vec3& world_position) {
	m_data->SetWorldPosition(world_position);
    PropagateParentPosition();
}

void CObject::SetParentPosition(const glm::vec3& parent_position) {
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

const glm::vec3& CObject::GetScaleFactor() const noexcept {
    return m_data->GetScaleLocalFactor();
}

void CObject::SetScaleValue(const float scale_value) {
	m_data->SetScaleLocalValue(scale_value);
}

void CObject::SetScaleFactor(const glm::vec3& factor) {
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
    if (m_children.find(child_name) == m_children.end())
	    return false;

    m_children.erase(child_name);
    return true;
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

} /*! opgs16::element */
} /*! opgs16 */

