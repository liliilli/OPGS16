#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::ScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class TestScript1 final : public opgs16::component::CScriptFrame {
public:
    TestScript1(opgs16::element::CObject& obj);

    virtual void Update() override final;

    inline void TriggerProcessFinish() {
        ++m_finished_obj;
    }

private:
    /*! Test sequence */
    enum class Sequence : size_t {
        _0_DO_NOTHING,          /*! The stage of before testing */
        _1_NORMAL_LOCAL,        /*! Local position translating stage. */
        _2_DEPENDENT_WORLD,     /*! World position translating stage. */
        _3_INDEPENDENT_WORLD,   /*! Same as _2_test but propagating flag toggled every 0.5s. */
        _4_ALPHA_BLENDING_TEST, /*! Every box object's alpha value will be changed. */
        _5_SCALING_TEST,        /*! Scaling test. */
        _6_ROTATION_TEST,       /*! The first rotation test. */
    } m_sequence{ Sequence::_0_DO_NOTHING };
    bool m_is_break{ true };    /*! The flag for break time */
    size_t m_switch{ 0 };
    size_t m_finished_obj{ 0 };

    TimerHandle m_timer_break;
    TimerHandle m_timer_third_test;
    TimerHandle m_timer_4_interval; /*! Timer waits for 10 seconds and proceed next stage. */
    TimerHandle m_timer_5_interval; /*! Timer waits for 10 seconds and proceed next stage. */

private:
    virtual void Start() override final;

    void SetBreakTimer();   /*! Set timer (m_timer_break) */
    void Resume();          /*! Callback function for m_timer_break */

    void ObjectPropertiesReset();
    void ChangeText();

    /*!
     * @brief MOVEMENT TEST FUNCTION 1
     *
     */
    void Proceed_1NormalLocal();

    /*!
     * @brief MOVEMENT TEST FUNCTION 2
     */
    void Proceed_2DependentWorld();

    void Proceed_3WorldPosition();
    void ToggleObjectProperties();
    void ResetObjectProperties();

    void Proceed_4AlphaBlending();
    void OnTrigger4Interval();

    void Proceed_5ScalingTest();
    void OnTrigger5Interval();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, TestScript1)
};

