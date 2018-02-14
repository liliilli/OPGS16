#include "debug_canvas.h"
#include <iostream>

#include "obj_fps.h"            /*! ObjectFps */
#include "obj_date.h"           /*! ObjectDate */
#include "obj_tree.h"           /*! ObjectObjectTree */
#include "Script\ui_manager.h"  /*! component::ScriptFrame
                                  * DebugUiManager script */
#include "Script\test_script.h"
#include "..\..\GlobalObjects\camera_object.h"  /*! For base class's constructor. */

CanvasDebug::CanvasDebug() {
    /*! Object binding */
    Instantiate<ObjectFps>("Fps");
    Instantiate<ObjectDate>("Date");
    Instantiate<ObjectObjectTree>("Hier");

    /*! Script binding */
    AddComponent<DebugUiManager>(*this,
        static_cast<::Canvas::Text*>(GetChild("Fps")),
        static_cast<::Canvas::Text*>(GetChild("Date")),
        static_cast<::Canvas::Text*>(GetChild("Hier")));

    AddComponent<TestScript>(*this);

    auto ptr = GetComponent<TestScript>();
    if (ptr) {
        std::cout << typeid(ptr).name() << '\n';
    }

    RemoveComponent<TestScript>();
}

void CanvasDebug::Update() {
    for (auto& script : m_components) {
        script->Update();
    }
    Canvas::Update();
}

void CanvasDebug::Draw() {
    Canvas::Draw();
}
