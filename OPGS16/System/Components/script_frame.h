#ifndef OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
#define OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H

/*!
 * @file System\Components\script_frame.h
 * @brief Base script frame file.
 * @author Jongmin Yun
 * @date 2018-02-13
 */

#include <string>                           /*! std::string as member */
#include "_macro.h"                         /*! SET_UP_HASH_MEMBER_DERIVED() */
#include "component.h"                      /*! Component as base class */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! Object as reference */

/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {

/*!
 * @class ScriptFrame
 * @brief The most base script class, which is derived by all user-defined script class.
 * ScriptFrame inherits component::Component class the most base class of all components.
 *
 * All derived class must be a final, and define Start() and Update() member function.
 * Initiate() and Start() must be called only once in initialization time.
 */
class ScriptFrame : public component::Component {
public:
    ScriptFrame(Object& bound_obj) : m_bound_object{ bound_obj } {}

    /*!
     * @brief This method will be called only one time, when ScriptFrame is attached to object,
     * and actived first time in game applicaiton.
     * This is just optional, you can do not to override Initiate() method when there is no need.
     */
    [[noreturn]] virtual void Initiate() {};

    /*!
     * @brief This method must be called in Initialization time of script bound to object.
     * And all derived scripts have to override this, even though not have components to set up.
     */
    [[noreturn]] virtual void Start() = 0;

    /*!
     * @brief This method updates information of script instance bound to specific object.
     * Must be overriden and implemented by derived script class.
     */
    [[noreturn]] virtual void Update() = 0;

    /*!
     * @brief This method will be called only one time, when the object are destroyed.
     * Object's destuctor call all destroy() methods in all scripts bound to object to be vanished.
     * This is optional method, you could leave it vacant.
     */
    [[noreturn]] virtual void Destroy() {};

private:
    const std::string m_script_name;    /*! Name must be same as ScriptFrame derived. */
    Object& m_bound_object;             /*! Bound object which script instance refers to */

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::Component, ScriptFrame)
};

}

#endif // !OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
