#ifndef OPENGL_TUTORIAL_OBJECTS_WOOD_BOX_H
#define OPENGL_TUTORIAL_OBJECTS_WOOD_BOX_H
#include "..\helper.h"
#include "..\object.h"
#include "..\texture.h"

/**
 * @file wood_box.h
 * @brief Just wooden box!
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

class WoodBox : public Object {
    WoodBox();

    /**
     * @brief The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader) override;

private:
    helper::BindingObject object_info;

    texture::Texture2D texture { "Resources/container.png", GL_RGBA };
    texture::Texture2D texture_specular { "Resources/container_spec.png", GL_RGBA };
};


#endif /** OPENGL_TUTORIAL_OBJECTS_WOOD_BOX_H */