#ifndef OPGS16_SYSTEM_COMPONENTS__MACRO_H
#define OPGS16_SYSTEM_COMPONENTS__MACRO_H

/*! Preprocessor meta programming */

#define TO_STRING(__TYPE__) #__TYPE__   /*! Convert arguement to string literal */

/*!
 * @macro SET_UP_HASH_VALUE_OF
 * @brief Set __TYPE__ class, inherits Component class or any derived class have type value,
 * which is a hash value of string of __TYPE__ itself.
 * This macro must be added to _type_hash.h file, whenever any derived class was created.
 *
 * @param[in] __TYPE__ child type value of the top of parent class which is component::Component.
 * @dependency STL utility, string header file.
 */
#define SET_UP_HASH_VALUE_OF(__TYPE__)                                                      \
const size_t __TYPE__::type = std::hash<std::string>{}(TO_STRING(__TYPE__));

/*!
 * @macro SET_UP_HASH_MEMBERS_DERIVED
 * @brief Declare type member variable, which is used for storing hashed type value.
 */
#define SET_UP_HASH_MEMBERS_DERIVED()                                                   \
public: static const size_t type;

/*!
 * @macro OVERRIDE_TYPEMATCH
 * @brief Define override function of component::Component's DoesTypeMatch() to match with
 * new type value of __DERIVED__ type. Defined function does not throw exception.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)                                       \
public:                                                                                 \
inline virtual bool DoesTypeMatch(const size_t type_val) const noexcept override {      \
    if (__DERIVED__::type == type_val)                                                  \
        return true;                                                                    \
    else return __BASE__::DoesTypeMatch(type_val);                                      \
}

/*!
 * @macro SET_UP_TYPE_MEMBER
 * @brief Automaticallly create and define class members related to hashed type value, together.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define SET_UP_TYPE_MEMBER(__BASE__, __DERIVED__)                                       \
SET_UP_HASH_MEMBERS_DERIVED()                                                           \
OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)

#endif // !OPGS16_SYSTEM_COMPONENTS__MACRO_H
