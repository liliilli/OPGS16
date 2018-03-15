#pragma once

#include "../../../../Headers/script_common.h"     /*! component::ScriptFrame */
#include "../../../../System/Frame/timer_handle.h" /*! TimerHandle */

class McScript final : public opgs16::component::CScriptFrame {
public:
    McScript(opgs16::element::CObject& obj);

    virtual void Update() override final;

private:
    virtual void Start() override final;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, McScript)
};

