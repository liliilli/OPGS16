#ifndef OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H
#define OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H

/*!
 * @file Objects\Debug\Script\ui_manager.h
 * @author Jongmin Yun
 * @date 2018-02-13
 */

#include "..\..\..\Headers\script_common.h"
/*!
 * SET_UP_HASH_MEMBER_DERIVED()
 * component::ScriptFrame
 * Canvas::Text * Object * ObjectTree
 */

class DebugUiManager final : public component::ScriptFrame {
public:
    DebugUiManager(Object& obj, Canvas::Text* const, Canvas::Text* const, Canvas::Text* const);

    virtual void Update() override final;

private:
    virtual void Start() override final;

    /*! This components must not be a dangling pointer! */
    Canvas::Text* m_fps;
    Canvas::Text* m_date;
    Canvas::Text* m_tree;

private:
    void SetHierarchyText(const ObjectTree* item, size_t count, std::string* const text);

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, DebugUiManager)
};

#endif // !OPGS16_OBJECTS_DEBUG_SCRIPT_UI_MANAGER_H
