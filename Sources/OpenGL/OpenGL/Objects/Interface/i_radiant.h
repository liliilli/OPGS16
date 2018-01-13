#ifndef OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_RADIANT_H
#define OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_RADIANT_H

/**
 * @file i_radiant.h
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include "..\..\shader.h"

/**
 * @interface IRadiant
 * @brief IRadiant type let derived class set light information which is implemented on
 * derived class up to shader.
 */
class IRadiant {
public:
    virtual ~IRadiant() = default;

    /**
     * @brief The method sets up light information to shader.
     * @param[in] shader Shader to set up light information
     */
    [[noreturn]] virtual void SetUpLight(const int index, helper::ShaderNew& shader) = 0;
};

#endif /** OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_RADIANT_H */