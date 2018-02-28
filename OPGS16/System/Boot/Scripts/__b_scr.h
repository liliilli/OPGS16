#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::ScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class __B_SCR final : public component::ScriptFrame {
public:
    __B_SCR(Object& obj);
    virtual void Update() override final {};

private:
    TimerHandle m_timer_break;

private:
    virtual void Start() override final;

    void OnTriggerTimerBreak();
    void PlaySoundEffect();
    void CreateTextObject();

    void OnTriggerNextScene();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, __B_SCR)
};

