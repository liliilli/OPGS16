#include "ui_manager.h"
#include <chrono>                                   /*! std::chrono::system_clock */
#include <ctime>                                    /*! std::time_t */
#include <iomanip>                                  /*! std::setprecision */
#include <sstream>                                  /*! std::ostringstream */
#include "../../../GlobalObjects/Canvas/text.h"     /*! Canvas::Text */
#include "../../../System/Manager/Public/time_manager.h"   /*! opgs16::manager::MTimeManager */

/*! Tree */
#include "../../../System/Debugs/hierarchy_tree.h"  /*! ObjectTree */
#include "../../../System/Frame/scene.h"            /*! GetObjectTree() */
#include "../../../System/Manager/Public/scene_manager.h"  /*! MSceneManager */

DebugUiManager::DebugUiManager(opgs16::element::CObject& obj,
                               canvas::Text* const _m_fps,
                               canvas::Text* const _m_date,
                               canvas::Text* const _m_tree) :
    opgs16::component::CScriptFrame(obj),
    m_fps{ _m_fps }, m_date{ _m_date }, m_tree{ _m_tree } {
    Initiate();
    Start();
}

void DebugUiManager::Start() {}

void DebugUiManager::Update() {
    if (m_fps) {
        std::ostringstream str;
        str << std::setprecision(4) << opgs16::manager::MTimeManager::Instance().GetFpsSeconds();
        m_fps->SetText("Fps : " + str.str());
    }

    if (m_date) {
        auto timepoint = std::chrono::system_clock::now();
        std::time_t time_struct = std::chrono::system_clock::to_time_t(timepoint);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&time_struct), "%F %T");
        m_date->SetText(stream.str());
    }
}
//
//void DebugUiManager::SetHierarchyText(const ObjectTree* item,
//                                      size_t count,
//                                      std::string* const text) {
//    if (count == 0) text->append("Scene\n");
//    else {
//        std::string space_text{};
//        for (size_t i = 1; i <= count; ++i) { space_text.push_back(' '); }
//        text->append(space_text + item->name + '\n');
//    }
//
//    for (const auto& child : item->children) {
//        SetHierarchyText(&child, count + 1, text);
//    }
//}

