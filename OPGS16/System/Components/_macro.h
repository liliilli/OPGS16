#ifndef SYSTEM_COMPONENTS__MACRO_H
#define SYSTEM_COMPONENTS__MACRO_H

/*! Preprocessor meta programming */
#define TO_STRING(__TYPE__) #__TYPE__

/*!
 * @macro
 * @brief
 * @param[in] __TYPE__
 */
#define SET_UP_TYPE_HASH(__TYPE__)                                                      \
const size_t __TYPE__::type = std::hash<std::string>{}(TO_STRING(__TYPE__));

/*!
 * @macro SET_UP_HASH_MEMBERS_DERIVED
 * @brief
 */
#define SET_UP_HASH_MEMBERS_DERIVED()                                                   \
public: inline virtual const size_t Type() noexcept override { return type; }           \
public: static const size_t type;

/*!
 * @macro OVERRIDE_TYPEMATCH
 * @brief
 * @param[in] __BASE__
 * @param[in] __DERIVED__
 */
#define OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)                                       \
public:                                                                                 \
inline virtual bool DoesTypeMatch(const size_t type_val) const noexcept override {      \
    if (__DERIVED__::type == type_val)                                                  \
        return true;                                                                    \
    else return __BASE__::DoesTypeMatch(type_val);                                      \
}

#endif // !SYSTEM_COMPONENTS__MACRO_H
