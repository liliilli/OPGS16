#ifndef OPENGL_TUTORIAL_SCENE_H
#define OPENGL_TUTORIAL_SCENE_H

/**
* @file scene.h
* @brief The file consist of basic scene API.
*
* All derived class based on Scene class can be used parameter of Application::PushScene(). 
*
* @author Jongmin Yun
* @version 0.0.1
*/

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "camera.h"
#include "font.h"
#include "helper.h"

/**
 * @class Scene
 * @brief Base scene interface.
 */
class Scene {
public:
    Scene();
    virtual ~Scene() = default;

    /**
     * @brief The method handles key codes from window.
     *
     * This methods is called by Application::ProcessInput method.
     *
     * @param[in] window Window handle pointer.
     */
    [[noreturn]] virtual void HandleInput(GLFWwindow* const window);

    /**
     * @brief The method update components movement, UI refresh, and so on.
     */
    [[noreturn]] virtual void Update();
    
    /**
     * @brief The method calls scene to draw all objects.
     */
    [[noreturn]] virtual void Draw();

protected:
    camera::Camera& camera;

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

#endif // OPENGL_TUTORIAL_SCENE_H

