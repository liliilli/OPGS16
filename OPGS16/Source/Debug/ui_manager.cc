
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
///
///

#include <Debug\ui_manager.h>   /// Header file

#include <chrono>   /// std::chrono::system_clock
#include <ctime>    /// std::time_t
#include <iomanip>  /// std::setprecision
#include <sstream>  /// std::ostringstream

#include <Element\Canvas\text.h>    /// ::opgs16::element::canvas::CText
#include <Manager\time_manager.h>   /// ::opgs16::maanger::MTimeManager

DebugUiManager::DebugUiManager(opgs16::element::CObject& obj,
                               opgs16::element::canvas::CText* const _m_fps,
                               opgs16::element::canvas::CText* const _m_date,
                               opgs16::element::canvas::CText* const _m_tree) :
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