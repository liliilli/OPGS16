#ifndef OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H
#define OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H

/*!
 * @file System\Components\Physics2D\Collider
 * @author Jongmin Yun
 * @date 2018-02-14
 */

#include <list>
#include "..\_macro.h"
#include "..\component.h"
#include "..\..\..\Headers\Fwd\objectfwd.h" /*! Object as reference */
#include "collider_2d.h"    /*! Collider2D */

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
class Rigidbody2D final : component::Component {
public:
    enum class BodyType {
        NORMAL,     /*! Move it with physics simulation */
        STATIC      /*! Move move with */
    };

public:
    Rigidbody2D(Object& bound_obj) : m_bound_object{ bound_obj } {}

    /*!
     * Update physics/collision process.
     */
    [[noreturn]] void Update();

private:
    Object& m_bound_object;             /*! Bound object which script instance refers to */

    bool m_simulated{ true };           /*! If you want to simulate physics/collision wit this,
                                         *  you have to set it on true. */
    float m_object_mass{ 1.0f };        /*! Define the mass of the Rigidbody2D */
    float m_gravity_scale{ 1.0f };      /*! The degree to which the object affected by gravity */

    BodyType m_type{ BodyType::NORMAL };
    std::list<collision::Collider2D> m_colliders{};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::Component, Rigidbody2D)
};

}

#endif //! OPGS16_PHYSICS_SYSTEM_RIGIDBODY_2D_H