#pragma once

/*!
 * @file Maintenance\Object\test_obj.h
 * @author Jongmin Yun
 * @date 2018-02-23
 * @log
 * 2018-02-23 Fuck
 */

#include "..\..\System\Object\object.h"             /*! Object */
#include "../../System/Components/Public/sprite_renderer.h"    /*! Sprite2DRenderer */

class TestObject1 final : public Object {
    using Sprite2DRenderer = opgs16::component::Sprite2DRenderer;

public:
    TestObject1(const int i, const float size);

    Sprite2DRenderer& GetRenderer() {
        return m_sprite_renderer;
    }

private:
    Sprite2DRenderer m_sprite_renderer;   /*! Sprite Renderer */

    virtual void Render() override final;
};