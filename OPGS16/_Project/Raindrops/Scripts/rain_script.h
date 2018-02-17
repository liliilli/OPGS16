#pragma once

#include "..\..\..\Headers\script_common.h"
#include <random>   /*! Random */

class RainScript final : public component::ScriptFrame {
public:
    RainScript(Object& obj);

    virtual void Update() override final;

private:
    InputManager& m_input;
    std::random_device m_rng;

private:
    virtual void Start() override final;

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, RainScript)
};

