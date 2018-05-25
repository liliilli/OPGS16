
#include "../../Include/Object/lifecycle_manager.h"

/// ::debug::script::LifecycleManager
#include "../../Include/Script/lifecycle_manager.h"

namespace debug::object {

LifecycleManager::LifecycleManager() {
  AddComponent<script::LifecycleManager>(*this);
}

} /// ::debug::object
