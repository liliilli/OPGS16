#pragma once

#include "..\..\..\Headers\script_common.h"     /*! component::ScriptFrame */
#include "..\..\..\System\Frame\timer_handle.h" /*! TimerHandle */

class TestScript1 final : public component::ScriptFrame {
public:
    TestScript1(Object& obj);

    virtual void Update() override final;

    inline void TriggerProcessFinish() {
        ++m_finished_obj;
    }

private:
    enum class Sequence : size_t {
        _0_DO_NOTHING,
        _1_NORMAL_LOCAL,
        _2_DEPENDENT_WORLD,
        _3_INDEPENDENT_WORLD,
    } m_sequence{ Sequence::_0_DO_NOTHING };
    bool m_is_break{ true };
    size_t m_switch{ 0 };
    size_t m_finished_obj{ 0 };

    TimerHandle m_timer_break;
    TimerHandle m_timer_third_test;

private:
    virtual void Start() override final;

    void SetTimer();/*! Set timer (m_timer_break) */
    void Resume();  /*! Callback function for m_timer_break */

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

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, TestScript1)
};

