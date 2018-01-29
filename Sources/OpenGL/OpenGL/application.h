#ifndef OPENGL_TUTORIAL_APPLICATION_H
#define OPENGL_TUTORIAL_APPLICATION_H

/**
 * @file application.h
 * @brief The main file to run application.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "System\Frame\scene.h"
#include "System\Shader\shader.h"
#include "System\Shader\pp_manager.h"
#include "GlobalObjects\Canvas\canvas.h"

/**
 * @class Application
 * @brief Application class runs application.
 *
 * This class is singleton, and must be initialized by calling getInstance method
 * to operate program.
 */
class Application {
public:
    /**
     * @brief Static method gets unique instance of Application class.
     */
    static Application& getInstance(std::string&& title = {}) {
        static Application instance{ std::move(title) };
        return instance;
    }

    /**
     * @brief Let application run and loop.
     */
    [[noreturn]] void Run();

    /**
     * @brief The method replace scene with old scene.
     */
    template <class _Ty, typename = std::enable_if_t<std::is_base_of<Scene, _Ty>::value>>
    [[noreturn]] void ReplaceScene(){
        PushScene<_Ty>();
    }

private:
    /** screen width, height */
    unsigned SCREEN_WIDTH   = 720u;
    unsigned SCREEN_HEIGHT  = 480u;

    /** Window handle pointer */
    GLFWwindow* window{ nullptr };

    std::stack<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> top_scene;

	std::unique_ptr<Object> m_canvas;

    bool aa_toggled{ false };
    bool fps_toggled{ false };
	bool post_processing_convex_toggled{ true };

    float old_time{};
    float new_time{};
    float elapsed_time{};
    float interval_time{};

    /** Time value for displaying text when fps_toggled is true. */
    float display_time{};

    /** Font instance for global text displaying */
    std::unordered_map<int, bool> pressed_key_map;

	shading::PostProcessingManager* m_pp_manager = &shading::PostProcessingManager::GetInstance();

private:
    explicit Application(std::string&& app_name = "Application");

    /**
     * @brief The method that initiates application.
     *
     * Initiate glfw, glew, create window and return window object pointer.
     *
     * @param[in] app_name Application title name.
     * @return Window handle pointer.
     */
    GLFWwindow* InitApplication(std::string&& app_name);

	/**
	 * @brief Initiate and Make font informations.
	 */
	[[noreturn]] void InitiateFonts();

	/**
	 * @brief Initiate and Compose Debug Interface components.
	 */
	[[noreturn]] void InitiateDebugUi();

	/**
	 * @brief Initiate post-processing effects in advance.
	 */
	[[noreturn]] void InitiatePostProcessingEffects();

	/**
	 * @brief Set sound.
	 */
	[[noreturn]] void InitiateSoundSetting();

	bool CheckSoundError();

    /**
     * @brief Static callback method for size checking and resizing.
     *
     * @param[in] window Window handle pointer.
     * @param[in] width Width to be resized.
     * @param[in] height Height to be resized.
     */
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    /**
     * @brief The method polls any key inputs.
     *
     * This methods polls and detects any key inputs, and process global key events.
     * If no input events match, keycode is brought to scene displaying on screen.
     *
     * @param[in] window Window handle pointer.
     */
    [[noreturn]] void ProcessInput(GLFWwindow* const window);

    /**
     * @brief The method that adds scene to scene stack.
     *
     * Add scene to scene stack stores scenes is paused, and move to top scene.
     *
     * @param[in] _Ty* Type parameter is based on Scene, value must be nullptr to prevent
     * Double initiation of scene.
     */
    template <class _Ty>
    [[noreturn]] void PushScene() {
        scenes.push(std::make_shared<_Ty>());
        top_scene = scenes.top();
    }

    /**
     * @brief The method remove scene.
     */
    [[noreturn]] void PopScene();

    /**
     * @brief The method update components movement, UI refresh, and so on.
     */
    [[noreturn]] void Update();

	void UpdateDebugInformation();

    /**
     * @brief The method calls scene to draw all objects.
     */
    [[noreturn]] void Draw();

	[[noreturn]] void DrawDebugInformation();

private:
    /**
     * @brief Helper method that checks if keycode was pressed.
     *
     * @param[in] window Window handle pointer.
     * @param[in] keycode Code of key to be checked.
     *
     * @return If keycode is pressed, return true. otherwise false.
     */
    bool DoesKeyPressed(GLFWwindow* const window, const int keycode) {
        if (glfwGetKey(window, keycode) == GLFW_PRESS &&
            (pressed_key_map.find(keycode) != pressed_key_map.end() &&
             pressed_key_map.at(keycode) == false)) {

            pressed_key_map[keycode] = true;
            return true;
        }
        else if (glfwGetKey(window, keycode) == GLFW_RELEASE) {
            pressed_key_map[keycode] = false;
            return false;
        }
		else {
			return false;
		};
    }

    /**
     * @brief The method toggles OpenGL antialiasing (MSAA)
     */
    [[noreturn]] void ToggleAntialiasing();

    /**
     * @brief The method toggles FPS display.
     */
    [[noreturn]] void ToggleFpsDisplay();

    /**
     * @brief Helper method that checks if next frame is set when V-sync is on.
     * @return If next frame is set, return true. else false.
     */
    bool IfFrameTurned();

    /**
     * @brief The methods set fps (frame per seconds);
     *
     * This methods can be called anytime use wants.
     * But when v-sync is off, this methods would be useless.
     *
     * @param[in] hz Herz
     */
    [[noreturn]] void SetFps(float hz);
};

#endif // OPENGL_TUTORIAL_APPLICATION_H