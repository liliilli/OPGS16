#include "resource_type.h"

namespace resource {

ScopeType GetScopeType(char type) {
    switch (type) {
    default: break;
    case 'G': return ScopeType::GLOBAL;      break;
    case 'L': return ScopeType::SCENE_LOCAL; break;
    }
}

}
