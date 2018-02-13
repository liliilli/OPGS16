#ifndef SYSTEM_COMPONENTS_COMPONENT_H
#define SYSTEM_COMPONENTS_COMPONENT_H

/*!
 * @file System\Components\component.h
 * @brief
 */

/*!
 * @namespace component
 * @brief
 */
namespace component {

/*!
 * @class Component
 * @brief
 */
class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    /*!
     * @brief Return type hash value.
     * This method used to match component type's hashed value and Class value.
     * @return Type hashed value of this class instance.
     */
    inline virtual const size_t Type() noexcept { return type; }

    /*!
     * @brief Return true/false flag whether or not your finding class is this.
     * @param[in] type_value Hashed type value of type which you want to find.
     * @return True/False flag, if you found proper class return true else false.
     */
    inline virtual bool DoesTypeMatch(const size_t type_value) const noexcept {
        return type == type_value;
    }

private:
    static const size_t type;
};

}

#endif // !SYSTEM_COMPONENTS_COMPONENT_H
