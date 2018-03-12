#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::CScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class ObjectScript1 final : public opgs16::component::CScriptFrame {
public:
    ObjectScript1(opgs16::element::CObject& obj);

    virtual void Update() override final;

    void DoWork(const size_t mode, const unsigned assigned_number) noexcept;

    void StopAllTimers();

private:
    bool        m_moving{ false };
    size_t      m_mode{ 0 };
    unsigned    m_switch{ 0 };
    unsigned    m_assigned_number{ 0 };

    float       m_object_alpha{ 1.0f };
    float       m_elapsed_time{ 0.0f };
    float       m_object_scale_offset{ 1.0f };
    float       m_object_original_scale{ 0.0f };
    const float m_2pi{ 3.1415926535f * 2 };

    TimerHandle m_timer_1_delay;    /*! Test 1 timers */
    TimerHandle m_timer_4_delay;    /*! Test 4 timers */
    TimerHandle m_timer_4_interval;
    TimerHandle m_timer_4_waiting;
    TimerHandle m_timer_5_delay;    /*! Test 5 timers */

    TimerHandle m_timer_swap;       /*! Swap texture index timer each 500ms */

private:
    virtual void Start() override final;

    inline void OnWork_1Switch() {
        m_moving = true;
    }

    void OnTriggered4Delay();

    void OnTriggered4Interval();

    void OnTriggered5Delay();

    void Proceed_1NormalLocal();

    void Proceed_3WorldPosition();

    void Proceed_4AlphaBlending();

    void Proceed_5Scaling();

    void OnTriggerSwap();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, ObjectScript1)
};

