#ifndef OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H
#define OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H

#include "..\..\..\System\Components\_macro.h"          /*! SET_UP_HASH_MEMBER_DERIVED() */
#include "..\..\..\System\Components\script_frame.h"    /*! component::ScriptFrame */
#include "..\..\..\Headers\Fwd\objectfwd.h"             /*! Canvas::Text
                                                          * Object
                                                          * ObjectTree */

class DebugUiManager final : public component::ScriptFrame {
public:
    DebugUiManager(Object& obj, Canvas::Text* const, Canvas::Text* const, Canvas::Text* const);

    virtual void Update() override final;


private:
    virtual void Start() override final;

    Canvas::Text* m_fps;
    Canvas::Text* m_date;
    Canvas::Text* m_tree;

private:
    void SetHierarchyText(const ObjectTree* item, size_t count, std::string* const text);

SET_UP_HASH_MEMBERS_DERIVED()           /*! Create members related to type hash value. */
OVERRIDE_TYPEMATCH(component::ScriptFrame, DebugUiManager)  /*! Override DoesTypeMatch(value) */
};

#endif // !OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H
