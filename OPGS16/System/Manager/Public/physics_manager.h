#ifndef OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H

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
 * @file System/Manager/Pubilc/physics_manager.h
 * @brief
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-15 Create file. Implement fundamental mechanism.
 * 2018-03-04 Refactoring.
 */

#include <memory>       /*! std::unique_ptr */
#include <vector>       /*! std::list */

#include "../Internal/physics_internal.h"
#include "../../../Headers/Fwd/objectfwd.h"    /*! component::Rigidbody2D */

namespace opgs16 {
namespace manager {

/*!
 * @class PhysicsManager
 * @brief Physics manager manages object collision and rigidbody movement.
 */
class PhysicsManager final {
    using item_ptr = std::unique_ptr<_internal::Item>;
    using item_raw = _internal::Item* ;

public:
    /*! Get Instance of PhysicsManager. */
    static PhysicsManager& Instance() {
        static PhysicsManager instance{};
        return instance;
    }

    /*!
     * @brief
     * @param[in]
     */
    void AddCollider(collision::RectangleCollider2D* const collider,
                     component::Rigidbody2D* const rigidbody);

    /*! Update and proceed collision routine */
    void Update();

    /*! Release and clean physics object container */
    void Clear();

private:
    std::vector<item_ptr> m_potential;
    std::vector<item_raw> m_active;

private:
    /*!
     * @brief 
     * @param[in]
     */
     void ProceedCollisionCheck(item_ptr& item);

    /*!
     * @brief
     * @param[in]
     */
     void EraseItem(item_ptr& item);
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
