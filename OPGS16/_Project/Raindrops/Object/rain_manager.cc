#include "rain_manager.h"
#include "..\Scripts\rain_script.h" /*! script::RainScript */

RainManager::RainManager() {
    AddComponent<RainScript>(*this);
}
