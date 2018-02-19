#ifndef OPGS16__PROJECT_RAINDROPS_SCRIPTS_RAIN_UI_H
#define OPGS16__PROJECT_RAINDROPS_SCRIPTS_RAIN_UI_H

#include "..\..\..\Headers\script_common.h"

class RainUiManager final : public component::ScriptFrame {
public:
    RainUiManager(Object& obj, Canvas::Text* const score);

    virtual void Update() override final;

    void AddScore(const unsigned value) {
        m_score_val += value;
    }

private:
    virtual void Start() override final;

    Canvas::Text* const m_score;
    unsigned m_score_val = 0;

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, RainUiManager)
};

#endif // !OPGS16__PROJECT_RAINDROPS_SCRIPTS_RAIN_UI_H
