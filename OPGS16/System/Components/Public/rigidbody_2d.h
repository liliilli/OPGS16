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
 * @file System/Components/Public/rigidbody_2d.h
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and Make declarations.
 * 2018-03-07 Move file from ::component to ::opgs16::component.
 */

#include <list>
#include <utility>
#include <memory>
#include "../Internal/component.h"
#include "../Internal/component_macro.h"
#include "../Physics2D/Collider/rectangle.h"    /*! collision::RectangleCollider2D */
#include "../../../Headers/Fwd/objectfwd.h"     /*! Object as reference */

namespace opgs16 {
namespace component {

/*!
 * @class Rigidbody2D
 * @brief There is only one Rigidbody class in each object. or undefined behavior occurs.
 */
class Rigidbody2D final : public _internal::Component {
public:
    enum class BodyType {
        NORMAL,     /*! Move it with physics simulation */
        STATIC      /*! Move move with */
    };

public:
    Rigidbody2D(element::Object& bound_obj) : _internal::Component{ bound_obj } {}

    /*!  * Update physics/collision process.  */
    void Update();

    /*!
     * @brief Add (insert) 2D collider object. This method could be used as just plain object
     * creation statement, except for noting collider type what you want to bind.
     * @param[in] _Ty 2D collider object, not just abstract Collider2D class.
     * @param[in] _Params varidic arguments, used to have been forwarding as constructor args.
     */
    template <
        class _Ty,
        class... _Params,
        typename = std::enable_if_t<std::is_base_of_v<collision::RectangleCollider2D, _Ty>>
    >    void AddCollider2D(_Params&&... args);

    template <
        class _Ty,
        typename = std::enable_if_t<std::is_base_of_v<collision::RectangleCollider2D, _Ty>>
    >    void AddCollider2D(std::unique_ptr<_Ty>&& collider);

     void OnCollisionEnter(Rigidbody2D& collier);

     void OnTriggerEnter(Rigidbody2D& collider);

    bool IsTag(const std::string&& tag) const;

    bool IsTag(const size_t index) const;

private:
    bool m_simulated{ true };           /*! If you want to simulate physics/collision wit this,
                                         *  you have to set it on true. */
    float m_object_mass{ 1.0f };        /*! Define the mass of the Rigidbody2D */
    float m_gravity_scale{ 1.0f };      /*! The degree to which the object affected by gravity */

    BodyType m_type{ BodyType::NORMAL };
    std::list<std::unique_ptr<collision::RectangleCollider2D>> m_colliders{};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::Component, Rigidbody2D)
};

template <class _Ty, class... _Params, typename>
 void Rigidbody2D::AddCollider2D(_Params&&... args) {
    m_colliders.emplace_back(std::make_unique<_Ty>( std::forward<_Params>(args)... ));
}

template <class _Ty, typename>
 void Rigidbody2D::AddCollider2D(std::unique_ptr<_Ty>&& collider) {
    m_colliders.emplace_back(std::move(collider));
}

} /*! opgs16::component */
} /*! opgs16 */

#endif //! OPGS16_SYSTEM_COMPONENTS_PUBLIC_RIGIDBODY_2D_H