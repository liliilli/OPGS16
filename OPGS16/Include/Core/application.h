#ifndef OPGS16_SYSTEM_CORE_APPLICATION_H
#define OPGS16_SYSTEM_CORE_APPLICATION_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file application.h
 * @brief Main file to run application.
 * This file consists of application operation class.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-01 Done refactoring following "studioN coding style convention".
 * 2018-03-11 Corection of ::opgs16::element::CObject class.
 *
 * @todo
 * 2018-03-01 Instantiate logger and bind it to Application.
 */

#include <memory>
#include <stack>
// ReSharper disable CppUnusedIncludeDirective
#include <GL/glew.h>
// ReSharper restore CppUnusedIncludeDirective
#include <GLFW/glfw3.h>

#include "../../Core/Public/logger.h"   /*! CLogger class running on separate thread. */
#include "../../Element/Public/object.h"     /*! ::opgs16::element::CObject */
#include "../../../Headers/Fwd/objectfwd.h"  /*! Many components */

/*! Framework overall namespace includes all components. */
namespace opgs16 {

/*!
 * @class MApplication
 * @brief Application class runs application.
 * This class is singleton, and must be initialized calling Instance member function
 * to operate actual program.
 *
 * @log
 * 2018-03-01 Move class inside opgs16 namespace for uniformation.
 *            Refactored and removed member functions are not appropriate for class.
 */
class MApplication final {
public:
    /*! Static method gets unique instance of Application class. */
    static MApplication& Instance() {
        static MApplication instance{};
        return instance;
    }

    /*! Let application initiate game setting. */
	void Initiate();

    /*! Let application run and loop. */
    void Run();

    /*! Set callback function will be called before update frame. */
    void SetOnBeforeUpdateCallback(std::function<void(void)> callback);

    inline const SGlobalSetting& Setting() const noexcept {
        return *m_setting;
    }

private:
    debug::CLogger&              m_logger;
    GLFWwindow*         m_window;           /*! Window handle pointer */

    manager::MSettingManager&     m_setting_manager;
	manager::MPostProcessingManager* m_pp_manager;
	manager::MInputManager&       m_input_manager;
    manager::MObjectManager&      m_object_manager;
    manager::MPhysicsManager&     m_physics_manager;
    manager::MResourceManager&    m_resource_manager;
    manager::MSceneManager&       m_scene_manager;    /*! SceneManager instance */
    manager::MSoundManager&       m_sound_manager;
    manager::MTimeManager&        m_time_manager;
    manager::MTimerManager&       m_timer_manager;

    std::unique_ptr<SGlobalSetting> m_setting;

	std::unique_ptr<element::CObject> m_debug_ui_canvas;	/*! Debug UI components container */
	std::unique_ptr<element::CObject> m_menu_ui_canvas;	/*! Global Menu UI components container */

    /*! This callback will be called before update routine only once. */
    std::function<void(void)> m_on_before_update_callback;

    /*! Brief global game status in this game application. */
    enum class GameStatus : size_t {
        INIT,	    /*! First, and Initial status in game application. */
        MENU,	    /*! Global Menu */
        PLAYING,    /*! Actual play mode, not paused, not menu. */
        EXIT,	    /*! Exit process from game application returning to game selection menu. */
        TERMINATE
    };

    std::stack<GameStatus> m_game_status;

private:
    MApplication();
    ~MApplication();

    /**
     * @brief The method that initiates application.
     * Initiate glfw, glew, create window and return window object pointer.
     *
     * @param[in] app_name Application title name.
     * @return Window handle pointer.
     */
    GLFWwindow* InitApplication(const std::string& app_name) const;

	/** Initiate and Make font informations. */
	void InitiateFonts() const;

	/** Initiate and Compose Debug Interface components.  */
	void InitiateDebugUi();

	/** Initiate post-processing effects in advance.  */
	void InitiatePostProcessingEffects();

    /*! The method polls any key inputs. */
    void Input();

	/** Global input checking method */
	void InputGlobal();

    /*! The method update components movement, UI refresh, and so on. */
    void Update();

    /*! The method calls scene to draw all m_object_list. */
    void Draw() const;

	/** Exit game */
    void Exit();

    /** Change window size. */
	void ChangeScalingOption(EScaleType value) const;

    /** The method toggles FPS display.  */
    void ToggleFpsDisplay() const;

	/** Toggle post-processing effects */
	void TogglePostProcessingEffect() const;

    /**
     * @brief Return present status.
     * @return GameStatus value on top of stack, m_global_game_status saves game status.
     */
    GameStatus GetPresentStatus();

    /**
     * @brief Replace present status to the other status.
     * @param[in] status New status value to replace present status with.
     */
    void ReplacePresentStatus(GameStatus status);

    /**
     * @brief Pile new status up game status stack.
     * Every routines refers to game status stack will look up new status.
     *
     * @param[in] status New status value to pile up onto status stack.
     */
    void PushStatus(GameStatus status);

    /**
     * @brief Pop(Halt) present status and return to previous status.
     * If there is no more status in stack, exit application automatically.
     *
     * Do not use this method when replace one value with the other status value,
     * This will crash game on playing.
     */
    void PopStatus();

public:
    MApplication(const MApplication&) = delete;
    MApplication& operator=(const MApplication&) = delete;
};

}; /*! opgs16 */

#endif // OPGS16_APPLICATION_H
