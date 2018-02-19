#pragma once

/*!
 * @file _Project\CollisionTest\Scripts\player.h
 * @author Jongmin Yun
 * @date 2018-02-17
 */

#include "..\..\..\Headers\script_common.h"

class Drops final : public component::ScriptFrame {
public:
    Drops(Object& obj) : component::ScriptFrame(obj) {
        Initiate();
        Start();
    }

    virtual void Update() override final;

    void ScoreUpTrigger();

private:
    size_t  m_count{ 5 };
    bool    m_trigger{ false };
    float   m_mass;
    float   m_velocity{ 0.f };
    float   m_acc{ 0.05f };

private:
    virtual void Start() override final {};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, Drops)
};
