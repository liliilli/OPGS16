#include "ui_manager.h"
#include <chrono>                                   /*! std::chrono::system_clock */
#include <ctime>                                    /*! std::time_t */
#include <iomanip>                                  /*! std::setprecision */
#include <iostream>                                 /*! std::cout */
#include <sstream>                                  /*! std::ostringstream */
#include "..\..\..\GlobalObjects\Canvas\text.h"     /*! Canvas::Text */
#include "../../../System/Manager/Public/time_manager.h"   /*! opgs16::manager::TimeManager */

/*! Tree */
#include "..\..\..\System\Debugs\hierarchy_tree.h"  /*! ObjectTree */
#include "..\..\..\System\Frame\scene.h"            /*! GetObjectTree() */
#include "../../../System/Manager/Public/scene_manager.h"  /*! SceneManager */

DebugUiManager::DebugUiManager(Object& obj,
                               canvas::Text* const _m_fps,
                               canvas::Text* const _m_date,
                               canvas::Text* const _m_tree) :
    component::ScriptFrame(obj),
    m_fps{ _m_fps }, m_date{ _m_date }, m_tree{ _m_tree } {
    Initiate();
    Start();
}

void DebugUiManager::Start() {}

void DebugUiManager::Update() {
    if (m_fps) {
        std::ostringstream str;
        str << std::setprecision(4) << opgs16::manager::TimeManager::Instance().GetFpsSeconds();
        m_fps->SetText("Fps : " + str.str());
    }

    if (m_date) {
        auto timepoint = std::chrono::system_clock::now();
        std::time_t time_struct = std::chrono::system_clock::to_time_t(timepoint);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&time_struct), "%F %T");
        m_date->SetText(stream.str());
    }

    if (false) {
        ObjectTree tree{};

        auto const top_scene = opgs16::manager::SceneManager::Instance().PresentScene();
        if (top_scene != nullptr) {
            top_scene->GetObjectTree(&tree);
            std::string text{};
            SetHierarchyText(&tree, 0, &text);
            m_tree->SetText(text);
        }
    }
}

void DebugUiManager::SetHierarchyText(const ObjectTree* item,
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

