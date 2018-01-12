#ifndef OPENGL_TUTORIAL_OBJECT_H
#define OPENGL_TUTORIAL_OBJECT_H

/**
 * @file object.h
 * @brief The file consist of basic scene API.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each objects you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include "camera.h"
#include "shader.h"

/**
 * @class Object
 * @brief The class for every object to update and draw.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each objects you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * Each object can be called using Update and Draw with shader to use.
 */
class Object {
public:
    Object();
    virtual ~Object();

    /**
     * @brief The method updates components of object.
     */
    [[noreturn]] virtual void Update() = 0;

    /**
     * @brief The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader) = 0;

protected:
    /** Caution :: Dangling reference! */
    camera::Camera& camera;
};

/**
 * @interface IControllable
 * @brief Interface for controllable object using key input
 */
class IControllable {
public:
    [[noreturn]] virtual void HandleInput(GLFWwindow* const window) = 0;

private:
    /**
     * @brief Helper method of scene that checks if keycode was pressed.
     *
     * @param[in] window Window handle pointer.
     * @param[in] keycode Code of key to be checked.
     *
     * @return If keycode is pressed, return true. otherwise false.
     */
    bool DoesKeyPressed(GLFWwindow* const window, const int keycode) {
        if (glfwGetKey(window, keycode) == GLFW_PRESS)
            return true;
        else
            return false;
    }
};

#endif /** OPENGL_TUTORIAL_OBJECT_H */