#ifndef OPGS16_SYSTEM_COMPONENTS_COMPONENT_H
#define OPGS16_SYSTEM_COMPONENTS_COMPONENT_H

/*!
 * @file System\Components\component.h
 * @author Jongmin Yun
 * @date 2018-02-13
 */

/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {

/*!
 * @class Component
 * @brief The most base class of derived component classes.
 */
class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    /*!
     * @brief Return true/false flag whether or not your finding class is this.
     * @param[in] type_value Hashed type value of type which you want to find.
     * @return True/False flag, if you found proper class return true else false.
     */
    inline virtual bool DoesTypeMatch(const size_t type_value) const noexcept {
        return type == type_value;
    }

public:
    static const size_t type;
};

}

#endif // !OPGS16_SYSTEM_COMPONENTS_COMPONENT_H
