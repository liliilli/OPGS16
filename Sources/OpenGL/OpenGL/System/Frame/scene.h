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

#include <memory>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "helper.h"
#include "object.h"
#include "..\font_manager.h"
#include "..\..\GlobalObjects\camera.h"

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
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Update() of objects.
     */
	[[noreturn]] virtual void Update();

    /**
     * @brief The method calls scene to draw all objects.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Draw() of objects.
     */
	[[noreturn]] virtual void Draw();

	template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InsertObject(const std::string&& tag, std::unique_ptr<_Ty>&& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
	}

	template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InsertObject(const std::string&& tag, std::unique_ptr<_Ty>& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
	}

	auto GetObjects()->std::unordered_map<std::string, std::unique_ptr<Object>>&;

	std::unique_ptr<Object>& GetObject(const std::string&& tag);

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

private:
    std::unordered_map<std::string, std::unique_ptr<Object>> objects;
};

#endif // OPENGL_TUTORIAL_SCENE_H

