#pragma once

#include "../../../../Headers/script_common.h"      /*! component::ScriptFrame */
#include "../../../../System/Frame/timer_handle.h"  /*! TimerHandle */
#include "../../../../Headers/Fwd/objectfwd.h"      /*! ::opgs16::component::CRigidbody2D */

class McScript final : public opgs16::component::CScriptFrame {
public:
    McScript(opgs16::element::CObject& obj);
    void Update() override final;

    /*! Whatever using getter/setter or not, being invaded encapsulation is same. */
    bool m_jump{ false };

private:
    void Start() override final;
    opgs16::component::CRigidbody2D* m_rigidbody;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, McScript)
};

