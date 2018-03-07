#pragma once

/*!
 * @file Objects\Debug\Script\test_script.h
 * @author Jongmin Yun
 * @date 2018-02-13
 */

#include "..\..\..\Headers\script_common.h"
/*!
 * SET_UP_HASH_MEMBER_DERIVED()
 * component::ScriptFrame
 * Canvas::Text * Object * ObjectTree
 */

#include <iostream>

class TestScript final : public opgs16::component::ScriptFrame {
public:
    TestScript(Object& obj) : opgs16::component::ScriptFrame(obj) {
        Initiate();
        Start();
    }

    virtual void Update() override final {
        std::cout << "TestScript Update\n";
    }

private:
    virtual void Start() override final {
        std::cout << "TestScript Start\n";
    }

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::ScriptFrame, TestScript)
};
