#pragma once

/*!
 * @file Maintenance\Object\test_obj.h
 * @author Jongmin Yun
 * @date 2018-02-23
 * @log
 * 2018-02-23 Fuck
 */

#include "..\..\System\Object\object.h"             /*! Object */
#include "..\..\System\Object\sprite_renderer.h"    /*! SpriteRenderer */

class TestObject1 final : public Object {
public:
    TestObject1(const int i, const float size);

    SpriteRenderer& GetRenderer() {
        return m_sprite_renderer;
    }

private:
    SpriteRenderer m_sprite_renderer;   /*! Sprite Renderer */

    virtual void Render() override final;
};