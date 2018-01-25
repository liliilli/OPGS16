#ifndef OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H
#define OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H

/**
 * @file light_box.h
 * @brief The box emits pointlight.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\Headers\common.h"
#include "..\GlobalObjects\light.h"
#include "Interface\i_radiant.h"

class LightBox : public Object, public IRadiant {
public:
    LightBox();

    /**
     * @brief The method updates components of object.
     */
    [[noreturn]] void Update() override {};

    /**
     * @brief The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]] void Draw(helper::ShaderNew& shader) override;

    /**
     * @brief
     */
    [[noreturn]] void SetUpLight(const int index, helper::ShaderNew& shader) override;

    /** ----------------------------------------------------------------------+
     * @commend Boiler-plate codes
     */

    [[noreturn]] void SetPosition(glm::vec3 position) override;

    /**
     * @brief The method sets up diffuse color, and specular color light.
     * @param[in] color Color to set up.
     */
    [[noreturn]] void SetColor(glm::vec3 color);

private:
    helper::BindingObject   object_info;
    light::PointLight       pointlight;
};

#endif /** OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H */