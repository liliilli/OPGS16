#pragma once

/*!
 * @file Maintenance\Object\test_obj.h
 * @author Jongmin Yun
 * @date 2018-02-23
 * @log
 * 2018-02-23 Fuck
 */

#include "..\..\System\Object\object.h"             /*! Object */

class TestObject1 final : public Object {
public:
    TestObject1(const int i, const float size);
private:
    virtual void Render() override final;

    ShaderWrapper* m_wrapper;
};