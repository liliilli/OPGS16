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
class Application final {
public:
    /**
     * @brief Static method gets unique instance of Application class.
     */
    static Application& getInstance(std::string&& title = {}) {
        static Application instance{ std::move(title) };
        return instance;
    }

	/** Let application initiate game settings */
	[[noreturn]] void Initiate();

    /** Let application run and loop.  */
    [[noreturn]] void Run();

    /**
     * @brief The method replace scene with old scene.
     */
    template <class _Ty, typename = std::enable_if_t<std::is_base_of<Scene, _Ty>::value>>
    [[noreturn]] void ReplaceScene(){
		pReplaceScene<_Ty>();
    }

	/**
	 * @brief Get default screen size (no scaling screen size)
	 * @return Width, height size array.
	 */
	std::array<unsigned, 2> GetDefaultScreenSize();

	/**
	 * @brief Get scale value
	 * @return Scale value, 1, 2, 3.
	 */
	const int GetScaleValue() const {
		return static_cast<int>(m_scale);
	}

private:
    /** screen width, height */
    unsigned SCREEN_WIDTH   = 256u;
    unsigned SCREEN_HEIGHT  = 224u;

	/**
	 * @brief Global game status in this game application.
	 */
	enum class GameStatus {
		INIT,	/** First, and Initial status in game application. */
		PAUSED, /** Paused in this game application */
		MENU,	/** Global Menu */
		PLAYING,/** Actual play mode, not paused, not menu. */
		EXIT	/** Exit process from game application returning to game selection menu. */
	};
	std::stack<GameStatus> m_global_game_status{};

    GLFWwindow* window{ nullptr };					/** Window handle pointer */

    std::stack<std::shared_ptr<Scene>> m_scenes;	/** Scene stack */
    std::shared_ptr<Scene> top_scene;

	std::unique_ptr<Object> m_debug_ui_canvas;		/** Debug UI components container */
	std::unique_ptr<Object> m_menu_ui_canvas;		/** Global Menu UI components container */

    bool m_aa_toggled;
    bool m_debug_toggled;
	bool m_post_processing_toggled;
	bool m_is_size_scalable;

	enum class OptionScale : int{
		X1_DEFAULT = 1,	/** Screen will be showed with 256x224 size */
		X2_DOUBLE = 2,	/** Screen will be showed with 512x448 size (personally perfect) */
		X3_TRIPLE = 3,	/** Screen will be showed with 768x672 size */
	} m_scale;

	struct TimeData {
		float old_time;
		float new_time;
		float elapsed_time;
		float interval;
	} m_timeinfo;
    float old_time{};
    float new_time{};
    float elapsed_time{};
    float interval_time{};

    /** Time value for displaying text when fps_toggled is true. */
    float fps_tick{};

    /** Font instance for global text displaying */
    std::unordered_map<int, bool> pressed_key_map;

	shading::PostProcessingManager* m_pp_manager{ nullptr };

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

	/** Initiate and Make font informations. */
	[[noreturn]] void InitiateFonts();

	/** * @brief Initiate and Compose Debug Interface components.  */
	[[noreturn]] void InitiateDebugUi();

	/** * @brief Initiate post-processing effects in advance.  */
	[[noreturn]] void InitiatePostProcessingEffects();

	/** * @brief Set sounds.  */
	[[noreturn]] void InitiateSoundSetting();

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
     * This methods polls and detects any key inputs, and process global key events.
     * If no input events match, keycode is brought to scene displaying on screen.
     *
     * @param[in] window Window handle pointer.
     */
    [[noreturn]] void Input(GLFWwindow* const window);

    /** The method update components movement, UI refresh, and so on. */
    [[noreturn]] void Update();

	/** Update debug information. */
	[[noreturn]] void UpdateDebugInformation();

    /** The method calls scene to draw all objects. */
    [[noreturn]] void Draw();

	/** Render debug information */
	[[noreturn]] void DrawDebugInformation();

    /**
     * @brief The method that adds scene to scene stack.
     * Add scene to scene stack stores scenes is paused, and move to top scene.
     *
     * @param[in] _Ty* Type parameter is based on Scene, value must be nullptr to prevent
     * double initiation of scene.
     */
    template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Scene, _Ty>>>
    [[noreturn]] void PushScene() {
        m_scenes.push(std::make_shared<_Ty>());
        top_scene = m_scenes.top();
    }

	/**
	* @brief The method replace scene with old scene.
	*/
	template <class _Ty, typename = std::enable_if_t<std::is_base_of<Scene, _Ty>::value>>
	[[noreturn]] void pReplaceScene() {
		/** Pop present scene */
		top_scene = nullptr;
		m_scenes.pop();
		/** Push present scene */
		PushScene<_Ty>();
	}

    /**
     * @brief The method removes top (present) scene.
	 * If there is no scene, exit application automatically.
	 * Otherwise all Update() and Rendering procedures delegates to previous scene.
     */
    [[noreturn]] void PopScene();

	/**
	 * @brief Return present status.
	 * @return GameStatus value on top of stack, m_global_game_status saves game status.
	 */
	GameStatus GetPresentStatus() { return m_global_game_status.top(); }

	/**
	 * @brief Replace present status to the other status.
	 * @param[in] status New status value to replace present status with.
	 */
	[[noreturn]] void ReplacePresentStatus(GameStatus status) {
		m_global_game_status.pop();
		m_global_game_status.push(status);
	}

	/**
	 * @brief Pile new status up game status stack.
	 * Every routines refers to game status stack will look up new status.
	 *
	 * @param[in] status New status value to pile up onto status stack.
	 */
	[[noreturn]] void PushStatus(GameStatus status) {
		m_global_game_status.push(status);
	}

	/**
	 * @brief Pop(Halt) present status and return to previous status.
	 * If there is no more status in stack, exit application automatically.
	 *
	 * Do not use this method when replace one value with the other status value,
	 * This will crash game on playing.
	 */
	[[noreturn]] void PopStatus() {
		m_global_game_status.pop();
		if (m_global_game_status.empty()) {
			while (!m_scenes.empty()) PopScene();
			Exit();
		}
	}

	/** Exit game */
	[[noreturn]] void Exit() {
		glfwSetWindowShouldClose(window, true);
	};

	/** Change window size. */
	[[noreturn]] void ChangeScalingOption(OptionScale value);

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

    /** The method toggles OpenGL antialiasing (MSAA) */
    [[noreturn]] void ToggleAntialiasing();

    /** * @brief The method toggles FPS display.  */
    [[noreturn]] void ToggleFpsDisplay();

	/** Toggle post-processing effects */
	[[noreturn]] void TogglePostProcessingEffect();

    /**
     * @brief Helper method that checks if next frame set when V-sync is on.
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