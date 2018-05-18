#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_HELPER_FUNCTION_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_HELPER_FUNCTION_H

#include "../../../../../Headers/import_logger.h"   /*! logger in debug mode */
#include "enum_types.h"
#include "../Public/__s_object_axis.h"

namespace opgs16::builtin::sample {

template <typename... _Args>
void GenerateCharacterObject(element::CScene* scene, const char* name, ECharacterType type, _Args... args) {
    static_assert(sizeof...(_Args) > 0, "No parameter pack.");
    scene->Instantiate<__S_OBJECT_AXIS>(name, type, std::forward<_Args>(args)...);
}

inline void GenerateCharacterObject(element::CScene* scene, const char* name, ECharacterType type) {
    scene->Instantiate<__S_OBJECT_AXIS>(name, type);
}

} /*! ::opgs16::builtin::sample */

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_INTERFACE_HELPER_FUNCTION_H