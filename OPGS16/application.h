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
#include <stack>                    /*! std::stack */
#include <string>
#include <type_traits>
#include "Headers\Fwd\objectfwd.h"  /*! GLFWwindow
                                      * InputManager
                                      * Object
                                      * ObjectTree
                                      * ObjectManager
                                      * PostProcessingManager
                                      * TimeManager
                                      * PhysicsManager
                                      */
#include "System\Object\object.h"   /*! Object for complete deleter of unique_ptr<Object> */

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
	 * @brief Get default screen size (no scaling screen size)
	 * @return Width, height size array.
	 */
	const std::array<unsigned, 2> GetDefaultScreenSize() const;

	/**
	 * @brief Get scale value
	 * @return Scale value, 1, 2, 3.
	 */
	const int GetScaleValue() const { return static_cast<int>(m_scale); }

private:
    /** screen width, height */
    unsigned SCREEN_WIDTH   = 256u;
    unsigned SCREEN_HEIGHT  = 224u;

    GLFWwindow* window{ nullptr };					/** Window handle pointer */
    SceneManager&   m_scene_instance;               /*! SceneManager instance */
	shading::PostProcessingManager* m_pp_manager{ nullptr };
	InputManager*   m_m_input{ nullptr };
    TimeManager*    m_m_time{ nullptr };
    PhysicsManager* m_physics_manager{ nullptr };
    ObjectManager*  m_object_manager{ nullptr };

	std::unique_ptr<Object> m_debug_ui_canvas;		/** Debug UI components container */
	std::unique_ptr<Object> m_menu_ui_canvas;		/** Global Menu UI components container */

	/**
	 * @brief Global game status in this game application.
	 */
	enum class GameStatus : size_t {
		INIT,	/** First, and Initial status in game application. */
		MENU,	/** Global Menu */
		PLAYING,/** Actual play mode, not paused, not menu. */
		EXIT,	/** Exit process from game application returning to game selection menu. */
        TERMINATE
	}; std::stack<GameStatus> m_game_status{};

	struct GlobalOption {
		bool anti_aliasing;
		bool debug_mode;
		bool post_processing;
		bool size_scalable;
	} m_option;

	enum class OptionScale : int{
		X1_DEFAULT = 1,	/** Screen will be showed with 256x224 size */
		X2_DOUBLE = 2,	/** Screen will be showed with 512x448 size (personally perfect) */
		X3_TRIPLE = 3,	/** Screen will be showed with 768x672 size */
	} m_scale;

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
    [[noreturn]] void Input();

	/** Global input checking method */
	[[noreturn]] void InputGlobal();

    /** The method update components movement, UI refresh, and so on. */
    [[noreturn]] void Update();

    /** The method calls scene to draw all objects. */
    [[noreturn]] void Draw();

	/**
	 * @brief Return present status.
	 * @return GameStatus value on top of stack, m_global_game_status saves game status.
	 */
	GameStatus GetPresentStatus() { return m_game_status.top(); }

	/**
	 * @brief Replace present status to the other status.
	 * @param[in] status New status value to replace present status with.
	 */
	[[noreturn]] void ReplacePresentStatus(GameStatus status) {
		m_game_status.pop();
		m_game_status.push(status);
	}

	/**
	 * @brief Pile new status up game status stack.
	 * Every routines refers to game status stack will look up new status.
	 *
	 * @param[in] status New status value to pile up onto status stack.
	 */
	[[noreturn]] void PushStatus(GameStatus status) {
		m_game_status.push(status);
	}

	/**
	 * @brief Pop(Halt) present status and return to previous status.
	 * If there is no more status in stack, exit application automatically.
	 *
	 * Do not use this method when replace one value with the other status value,
	 * This will crash game on playing.
	 */
	[[noreturn]] void PopStatus() {
        if (!m_game_status.empty()) {
            m_game_status.pop();
            if (m_game_status.empty()) Exit();
        }
	}

	/** Exit game */
    [[noreturn]] void Exit();

        /** Change window size. */
	[[noreturn]] void ChangeScalingOption(OptionScale value);

    /** The method toggles OpenGL antialiasing (MSAA) */
    [[noreturn]] void ToggleAntialiasing();

    /** * @brief The method toggles FPS display.  */
    [[noreturn]] void ToggleFpsDisplay();

	/** Toggle post-processing effects */
	[[noreturn]] void TogglePostProcessingEffect();
};

#endif // OPENGL_TUTORIAL_APPLICATION_H