#ifndef OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H
#define OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H

/**
 * @file light_box.h
 * @brief The box emits pointlight.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\helper.h"
#include "..\object.h"
#include "..\texture.h"
#include "Interface\i_radiant.h"

class LightBox : public Object, public IRadiant {
public:
    LightBox();

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

};

#endif /** OPENGL_TUTORIAL_OBJECTS_LIGHT_BOX_H */