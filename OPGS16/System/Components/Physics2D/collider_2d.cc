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

/*!
 * @file System/Components/Physics2D/collider_2d.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-17 Add CollisionLayer__ functions.
 */

#include "collider_2d.h"

#include <stdexcept>
#include "../../Manager/Public/setting_manager.h"   /*! ::opgs16::manager::MSettingManager */

namespace collision {
namespace {
using opgs16::manager::MSettingManager;

} /*! unnamed namespace */

void CCollider2D::SetCollisionType(ECollisionType type) {
	m_collision_type = type;
}

void CCollider2D::SetCollisionLayerIndex(unsigned layer_value) {
    if (layer_value >= MSettingManager::Instance().CollisionLayerList().size())
        throw std::runtime_error("collision layer value is bigger than capacity.");
    m_collision_layer_index = layer_value;
}

void CCollider2D::SetCollisionLayerName(const std::string& layer_name) {
    const auto& list = MSettingManager::Instance().CollisionLayerList();
    unsigned layer_value{ 0 };

    for (const auto& str : list) {
        if (str == layer_name) {
            m_collision_layer_index = layer_value;
            break;
        }
        else ++layer_value;
    }

    if (m_collision_layer_index != layer_value)
        throw std::runtime_error("did not find collision layer value mathces string.");
}

std::string CCollider2D::CollisionLayerStringOf() const {
    return MSettingManager::Instance().CollisionLayerList()[m_collision_layer_index];
}

}
