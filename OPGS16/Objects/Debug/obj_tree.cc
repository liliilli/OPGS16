#include "obj_tree.h"                           /*! Header file */
#include <string>
#include "..\..\application.h"                  /*! Application */
#include "..\..\System\Debugs\hierarchy_tree.h" /*! ObjectTree */
#include "..\..\System\Frame\scene.h"           /*! GetObjectTree() */

void ObjectObjectTree::Update() {
    ObjectTree tree{};

    auto* const top_scene = Application::getInstance().GetTopScene();
    if (top_scene != nullptr) {
        top_scene->GetObjectTree(&tree);
        std::string text{};
        SetHierarchyText(&tree, 0, &text);
        SetText(text);
    }
}

void ObjectObjectTree::SetHierarchyText(const ObjectTree* item,
                                        size_t count,
                                        std::string* const text) {
    if (count == 0) text->append("Scene\n");
    else {
        std::string space_text{};
        for (size_t i = 1; i <= count; ++i) { space_text.push_back(' '); }
        text->append(space_text + item->name + '\n');
    }

    for (const auto& child : item->children) {
        SetHierarchyText(&child, count + 1, text);
    }
}
