#pragma once

#include <random>   /*! Random */
#include "..\..\..\Headers\script_common.h"
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class RainScript final : public component::ScriptFrame {
public:
    RainScript(Object& obj);

    virtual void Update() override final;

private:
    InputManager& m_input;
    std::random_device m_rng;
    bool m_start{ false };

    TimerHandle m_respawn_timer;

private:
    virtual void Start() override final;

    void Recreate();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, RainScript)
};

