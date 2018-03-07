#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::ScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class TestScript2 final : public opgs16::component::ScriptFrame {
public:
    TestScript2(Object& obj);

    virtual void Update() override final;

private:
    opgs16::manager::InputManager& m_input;

    virtual void Start() override final {};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::ScriptFrame, TestScript2)
};

