
#include "../../../Include/Object/LifecycleTest/lifecycle_manager.h"

/// ::debug::script::LifecycleManager
#include "../../../Include/Script/LifecycleTest/lifecycle_manager.h"

namespace debug::object {

LifecycleManager::LifecycleManager() {
  AddComponent<script::LifecycleManager>(*this);
}

} /// ::debug::object
