#ifndef OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_CONTROLABLE_H
#define OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_CONTROLABLE_H
#include <gl\glew.h>
#include <GLFW\glfw3.h>

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

#endif /** OPENGL_TUTORIAL_OBJECTS_INTERFACE_I_CONTROLABLE_H */