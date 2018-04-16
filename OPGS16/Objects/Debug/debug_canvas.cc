#include "debug_canvas.h"
#include <iostream>

#include "obj_fps.h"            /*! ObjectFps */
#include "obj_date.h"           /*! ObjectDate */
#include "obj_tree.h"           /*! ObjectObjectTree */
#include "Script\ui_manager.h"  /*! component::CScriptFrame
                                  * DebugUiManager script */
#include "..\..\GlobalObjects\camera_object.h"  /*! For base class's constructor. */

CanvasDebug::CanvasDebug() {
    /*! Object binding */
    Instantiate<ObjectFps>("Fps");
    Instantiate<ObjectDate>("Date");
    Instantiate<ObjectObjectTree>("Hier");

    /*! Script binding */
    AddComponent<DebugUiManager>(*this,
        static_cast<opgs16::element::canvas::CText*>(GetChild("Fps")),
        static_cast<opgs16::element::canvas::CText*>(GetChild("Date")),
        static_cast<opgs16::element::canvas::CText*>(GetChild("Hier")));
}