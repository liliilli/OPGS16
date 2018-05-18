
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Debug\debug_canvas.h> /// Header file

#include <Debug\obj_date.h>
#include <Debug\obj_fps.h>
#include <Debug\obj_tree.h>
#include <Debug\ui_manager.h>

#include <Element\camera_object.h>  /// Basic camera object.

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