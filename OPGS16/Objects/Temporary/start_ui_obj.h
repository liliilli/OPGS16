#ifndef OBJECTS_TEMPORARY_START_UI_OBJECT_H
#define OBJECTS_TEMPORARY_START_UI_OBJECT_H

#include "..\..\System\Object\ui_object.h"  /*! UiObject as base class */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! InputManager */

class StartUiManager final : public UiObject {
public:
    StartUiManager();
    ~StartUiManager() = default;

private:
    InputManager& input_manager;

private:
    /*! The method updates components of this object and input check. */
    [[noreturn]] virtual void Update() override final;
};

#endif // !OBJECTS_TEMPORARY_START_UI_OBJECT_H
