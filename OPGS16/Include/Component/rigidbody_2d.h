#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_RIGIDBODY_2D_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_RIGIDBODY_2D_H

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
 * @file Components/rigidbody_2d.h
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and Make declarations.
 * 2018-03-07 Move file from ::component to ::opgs16::component.
 * 2018-03-12 Add gravity and accelation feature.
 * 2018-03-16 Add physics, collision activation switch.
 * 2018-03-20 Add frame movement value array. (zero-overhead)
 */

#include <array>    /*! std::array */
#include <list>
#include <utility>
#include <memory>

/// ::opgs16::component::_internal::CComponent
#include <Component\Internal\component.h>
#include <Component\Internal\component_macro.h> /// Component macroes
/// ::opgs16::component::collision::CRectangleCollider2D
#include <Component\Physics2D\Collider\rectangle.h>

#include <opgs16fwd.h>  /// Forward declration.

namespace opgs16 {
namespace component {

/*!
 * @class CRigidbody2D
 * @brief There is only one Rigidbody class in each object. or undefined behavior occurs.
 * @log
 * 2018-03-12 Add gravity and accelation feature.
 * 2018-03-16 Add physics, collision activation switch.
 */
class CRigidbody2D final : public _internal::CComponent {
public:
    enum class BodyType {
        NORMAL,     /*! Move it with physics simulation */
        STATIC      /*! Move move with */
    };

public:
    CRigidbody2D(element::CObject& bound_obj) : _internal::CComponent{ bound_obj } {}

    /*! Update physics/collision process.  */
    void Update();

    /*!
     * @brief Add (insert) 2D collider object. This method could be used as just plain object
     * creation statement, except for noting collider type what you want to bind.
     * @param[in] _Ty 2D collider object, not just abstract CCollider2D class.
     * @param[in] _Params varidic arguments, used to have been forwarding as constructor args.
     */
    template <
        class _Ty,
        class... _Params,
        typename = std::enable_if_t<std::is_base_of_v<physics::CRectangleCollider2D, _Ty>>
    >
    decltype(auto) AddCollider2D(_Params&&... args);

    template <
        class _Ty,
        typename = std::enable_if_t<std::is_base_of_v<physics::CRectangleCollider2D, _Ty>>
    >
    decltype(auto) AddCollider2D(std::unique_ptr<_Ty>&& collider);

    void OnCollisionEnter(CRigidbody2D& collier);

    void OnTriggerEnter(CRigidbody2D& collider);

    bool IsTag(const std::string&& tag) const;

    bool IsTag(const uint32_t index) const;

    /*! Activate or deactivate physics calculation mode */
    inline void ActivatePhysics(const bool value) {
        m_physics = value;
    }

    /*! Activate or decativate collision calculation mode */
    inline void ActivateCollsion(const bool value) {
        m_collidable = value;
    }

    inline void ActiveSolidification(const bool value) {
        m_solid = value;
    }

    inline bool IsSolid() const noexcept {
        return m_solid;
    }

    inline void SetStable(const bool value) {
        if (value) {
            m_velocity.x = 0; m_velocity.y = 0; m_velocity.z = 0;
            m_accelation.x = 0; m_accelation.y = 0; m_accelation.z = 0;
        }
        m_stable = value;
    }

    /*! Get movement vector values */
    inline glm::vec3& Movement() noexcept {
        return m_movement;
    }

    /*! Get accelation vector values */
    inline glm::vec3& Accelation() noexcept {
        return m_accelation;
    }

    /*! Get velocit vector values */
    inline glm::vec3& Velocity() noexcept {
        return m_velocity;
    }

private:
    bool m_stable{ false };             /*! Do not check collision when value is true. */
    bool m_collidable{ true };          /*! If you want to simulate physics/collision wit this,
                                         *  you have to set it on true. */
    bool m_physics{ false };            /*! If you want to simulate physics motion, turn it on. */

    float m_object_mass{ 1.0f };        /*! Define the mass of the Rigidbody2D */
    float m_gravity_scale{ 1.0f };      /*! The degree to which the object affected by gravity */
    float m_gravity{ 98.0f };            /*! Gravity factor */
    bool  m_solid{ false };             /*! Solid flag whether relocated by obstacle or not */

    /*! Velocity variable for moving bound object. */
    glm::vec3 m_velocity;
    /*! Accelation variable for moving bound object. */
    glm::vec3 m_accelation;
    /*! Movement value each frame. */
    glm::vec3 m_movement;

    BodyType m_type{ BodyType::NORMAL };
    std::list<std::unique_ptr<physics::CRectangleCollider2D>> m_colliders{};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRigidbody2D)
};

template <class _Ty, class... _Params, typename>
decltype(auto) CRigidbody2D::AddCollider2D(_Params&&... args) {
    return m_colliders.emplace_back(std::make_unique<_Ty>( std::forward<_Params>(args)... ));
}

template <class _Ty, typename>
decltype(auto) CRigidbody2D::AddCollider2D(std::unique_ptr<_Ty>&& collider) {
    return m_colliders.emplace_back(std::move(collider));
}

} /*! opgs16::component */
} /*! opgs16 */

#endif //! OPGS16_SYSTEM_COMPONENTS_PUBLIC_RIGIDBODY_2D_H