#ifndef OBJECTS_TEMPORARY_RAIN_DROP_H
#define OBJECTS_TEMPORARY_RAIN_DROP_H

/*!
 * @file Objects\Temporary\rain_drop.h
 * @author Jongmin Yun
 * @date 2018-02-17
 */

#include "..\..\System\Object\object.h"             /*! Object */
#include "..\..\System\Object\sprite_renderer.h"    /*! SpriteRenderer */

class RainDrop final : public Object {
public:
    RainDrop(const std::string& sprite_tag);

private:
    SpriteRenderer m_sprite_renderer;   /*! Sprite Renderer */

    virtual void Draw() override final;

    virtual void OnCollisionEnter(component::Rigidbody2D& body) override final;
};

#endif // !OBJECTS_TEMPORARY_RAIN_DROP_H
