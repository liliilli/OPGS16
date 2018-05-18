#ifndef OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file System/Manager/Pubilc/physics_manager.h
/// @brief
/// @author Jongmin Yun
///
/// @log
/// 2018-02-15 Create file. Implement fundamental mechanism.
/// 2018-03-04 Refactoring.
///

#include <memory>       /*! std::unique_ptr */
#include <vector>       /*! std::list */

/// ::opgs16::manager::_internal::Item
#include <Manager\Internal\physics_internal.h>
/// Forward declaration
#include <opgs16fwd.h>

namespace opgs16::manager {

/*!
 * @class MPhysicsManager
 * @brief Physics manager manages object collision and rigidbody movement.
 */
class MPhysicsManager final {
    using item_ptr = std::unique_ptr<_internal::Item>;
    using item_raw = _internal::Item* ;

public:
    /*! Get Instance of PhysicsManager. */
    static MPhysicsManager& Instance() {
        static MPhysicsManager instance{};
        return instance;
    }

    /*!
     * @brief
     * @param[in]
     */
    void AddCollider(physics::CRectangleCollider2D* const collider,
                     component::CRigidbody2D* const rigidbody);

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

#endif // !OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
