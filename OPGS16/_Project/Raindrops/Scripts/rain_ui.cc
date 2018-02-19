#include "rain_ui.h"                                /*! Header file */

#include <sstream>
#include <iomanip>

#include "..\..\..\GlobalObjects\Canvas\text.h"     /*! Canvas::Text */

RainUiManager::RainUiManager(Object& obj, Canvas::Text* const score) :
    component::ScriptFrame(obj), m_score{ score } {
    Initiate();
    Start();
}

void RainUiManager::Start() { }

void RainUiManager::Update() {
    if (m_score) {
        std::stringstream score_stream{ std::to_string(m_score_val) };
        std::string disp_string;
        score_stream >> std::setw(6) >> disp_string;

        m_score->SetText("SCORE\n" + disp_string);
    }
}