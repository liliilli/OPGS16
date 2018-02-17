#pragma once

/*!
 * @file _Project\CollisionTest\Scripts\player.h
 * @author Jongmin Yun
 * @date 2018-02-17
 */

#include "..\..\..\Headers\script_common.h"

class Player final : public component::ScriptFrame {
public:
    Player(Object& obj) : component::ScriptFrame(obj) {
        Initiate();
        Start();
    }

    virtual void Update() override final;

private:
    virtual void Start() override final {};

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::ScriptFrame, Player)
};
