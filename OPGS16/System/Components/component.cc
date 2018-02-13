#include "component.h"      /*! component::Header file */
#include "script_frame.h"   /*! component::ScriptFrame */
#include "..\..\Objects\Debug\Script\ui_manager.h"  /*! UiManager */

#include <functional>       /*! std::hash */
#include <string>           /*! std::string */
#include "_macro.h"         /*! Macros */

namespace component {

SET_UP_TYPE_HASH(Component)
SET_UP_TYPE_HASH(ScriptFrame)

}

SET_UP_TYPE_HASH(DebugUiManager)