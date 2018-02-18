#ifndef OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H
#define OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H

/*!
 * @file System\Components\Physics2D\Collider
 * @author Jongmin Yun
 * @date 2018-02-14
 */

#include <list>
#include <utility>
#include <memory>
#include "..\_macro.h"
#include "..\component.h"
#include "..\..\..\Headers\Fwd\objectfwd.h" /*! Object as reference */
#include "Collider\rectangle.h" /*! collision::RectangleCollider2D */

/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {

/*!
 * @class Rigidbody2D
 * @brief There is only one Rigidbody class in each object. or undefined behavior occurs.
 */
class Rigidbody2D final : public component::Component {
public:
    enum class BodyType {
        NORMAL,     /*! Move it with physics simulation */
        STATIC      /*! Move move with */
    };

public:
    Rigidbody2D(Object& bound_obj) : m_bound_object{ bound_obj } {}

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
    Object& m_bound_object;             /*! Bound object which script instance refers to */

    bool m_simulated{ true };           /*! If you want to simulate physics/collision wit this,
                                         *  you have to set it on true. */
    float m_object_mass{ 1.0f };        /*! Define the mass of the Rigidbody2D */
    float m_gravity_scale{ 1.0f };      /*! The degree to which the object affected by gravity */

    BodyType m_type{ BodyType::NORMAL };
    std::list<std::unique_ptr<collision::RectangleCollider2D>> m_colliders{};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::Component, Rigidbody2D)

private:
    /*!
     * @brief
     */
     void ReflectPositionToLastCollider();
};

template <class _Ty, class... _Params, typename>
 void Rigidbody2D::AddCollider2D(_Params&&... args) {
    m_colliders.emplace_back(std::make_unique<_Ty>( std::forward<_Params>(args)... ));
}

template <class _Ty, typename>
 void Rigidbody2D::AddCollider2D(std::unique_ptr<_Ty>&& collider) {
    m_colliders.emplace_back(std::move(collider));
}


}

#endif //! OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H