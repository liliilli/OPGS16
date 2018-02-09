#ifndef OPENGL_TUTORIAL_OBJECTS_QUAD_FLOOR_H
#define OPENGL_TUTORIAL_OBJECTS_QUAD_FLOOR_H
#include "..\Headers\common.h"

/**
 * @file quad_floor.h
 * @brief Just a floor as you see
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

class QuadFloor : public Object {
public:
    QuadFloor();

    /**
    * @brief The method updates components of object.
    */
    [[noreturn]] virtual void Update() override {};

    /**
    * @brief The method calls scene to one objects.
    * @param[in] shader Shader to use.
    */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader) override;

private:
    helper::BindingObject object_info;
    texture::Texture2D texture{ "Resources/hardwood.jpg", GL_RGB };
};

#endif