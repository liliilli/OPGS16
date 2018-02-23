#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::ScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class ObjectScript1 final : public component::ScriptFrame {
public:
    ObjectScript1(Object& obj);

    virtual void Update() override final;

    void DoWork(const size_t mode, const unsigned assigned_number) noexcept;

private:
    bool m_moving{ false };
    size_t m_mode{ 0 };
    unsigned m_switch{ 0 };
    unsigned m_assigned_number{ 0 };

    TimerHandle m_timer_1_delay;

private:
    virtual void Start() override final {};

    inline void Do_1Work() {
        m_moving = true;
    }

    /*!
     * @brief MOVEMENT TEST FUNCTION 1
     */
    void Proceed_1NormalLocal();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, ObjectScript1)
};

