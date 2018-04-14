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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Core/Private/application.cc
 * @brief Implementation file of ../Public/application.h
 * @log
 * 2018-04-14 Add file information comment and Removed unused <iostream> header file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/application.h"              /*! Header file */

#include <string_view>                          /*! std::string_view */
#include "../Public/core_header.h"              /*! Subsequential header files */
#include "../Public/core_setting.h"
#include "../../Boot/Scene/Public/__boot.h"     /*! opgs16::manifest::sample::boot */
#include "../../../Headers/import_logger.h"     /*! import loggers */
#include "../../../manifest.h"                  /*! opgs16::manifest */

#if defined(_INITIAL_SCENE_INCLUDE_PATH)
#include _INITIAL_SCENE_INCLUDE_PATH
#endif

namespace opgs16 {

namespace {
using namespace std::string_view_literals;
constexpr std::wstring_view g_global_resource_path{ L"_resource.meta"sv };
constexpr float k_fps_count = 60.f;

/*! Callback method for size check and resizing */
void OnCallbackFrameBufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

} /*! unnamed namespace */

MApplication::MApplication() :
    m_logger{ debug::CLogger::Instance() },
#if defined(_OPGS16_DEBUG_OPTION)
    m_window{ InitApplication("OPGS16 DEBUG MODE") },
#else
    #if defined (_CUSTOM_PROJECT)
        #if !defined (_APPLICATION_PROJECT_NAME)
            static_assert(false,
                          "Application project name is not active."
                          "Please uncomment or make macro _APPLICATION_PROJECT_NAME");
        #endif
        #if !(_APPLICATION_PROJECT_NAME + 0)
            static_assert(false, "Application project name is not valid. Check manifest file.");
        #endif
        #if defined (_APPLICATION_WINDOW_NAME)
            #if !(_APPLICATION_WINDOW_NAME + 0)
                static_assert(false, "Application window name is not valid, check manifest file.");
            #else
                m_window{ InitApplication(_APPLICATION_WINDOW_NAME) },
            #endif
        #else
            m_window{ InitApplication(_APPLICATION_PROJECT_NAME) },
        #endif
    #else
        static_assert(false, "Please turn on _CUSTOM_PROJECT macro to make window properly.");
    #endif
#endif
    m_setting_manager{ manager::MSettingManager::Instance() },
    m_input_manager{ manager::MInputManager::Instance() },
    m_object_manager{ manager::MObjectManager::Instance() },
    m_physics_manager{ manager::MPhysicsManager::Instance() },
    m_resource_manager{ manager::MResourceManager::Instance() },
    m_scene_manager{ manager::MSceneManager::Instance() },
    m_sound_manager{ manager::MSoundManager::Instance() },
    m_time_manager{ manager::MTimeManager::Instance() },
    m_timer_manager{ manager::MTimerManager::Instance() } {

    m_setting = std::make_unique<SGlobalSetting>();
	PushStatus(GameStatus::INIT);
}

GLFWwindow* MApplication::InitApplication(const std::string& app_name) const {
    glfwInit();

    /*! OpenGL Setting */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    PUSH_LOG_INFO(L"GLFW CONTEXT VERSION 4.3 Core.");

    const auto window = glfwCreateWindow(SGlobalSetting::ScreenWidth(),
                                         SGlobalSetting::ScreenHeight(),
                                         app_name.c_str(),
                                         nullptr, nullptr);
    if (!window) {
        PUSH_LOG_ERRO(L"Failed to create GLFW window. Application will terminate.");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetFramebufferSizeCallback(window, &opgs16::OnCallbackFrameBufferSize);
    //glfwSetCursorPosCallback(window, camera::MouseControl);

    glewInit();
    return window;
}

void MApplication::Initiate() {
    if (GetPresentStatus() == GameStatus::INIT) {
        /*! Initialize Global Setting. */
        m_resource_manager.ReadResourceFile(g_global_resource_path.data());

        /*! Initialize resource list. */
        m_time_manager.SetFps(k_fps_count);
        m_input_manager.Initialize(m_window);
        m_sound_manager.ProcessInitialSetting();

		InitiateFonts();
		InitiateDebugUi();
		InitiatePostProcessingEffects();

        /*! Set gl rendering options. */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        static_assert(manifest::k_size <= 3, "manifest k_size must be range from 1 to 3.");
        static_assert(manifest::k_size > 0,  "manifest k_size must be range from 1 to 3.");

        if constexpr (manifest::k_size == 1) ChangeScalingOption(EScaleType::X1);
        else if constexpr (manifest::k_size == 2) ChangeScalingOption(EScaleType::X2);
        else ChangeScalingOption(EScaleType::X3);

#if defined(_CUSTOM_PROJECT)
    #if defined(_RESOURCE_SETTING_FILE_PATH)
            m_resource_manager.ReadResourceFile(L"_Project/Maintenance/_meta/_resource.meta");
    #else
            static_assert(false, "Please set a path for _RESOURCE_SETTING_FILE_PATH);
    #endif

    #if defined(_RESOURCE_SETTING_FILE_PATH)
        #if defined(_INITIAL_SCENE_FULL_NAME)
            #if !_SHOW_BOOT_SCREEN
                    M_PUSH_SCENE(_INITIAL_SCENE_FULL_NAME, true);
                    ReplacePresentStatus(GameStatus::PLAYING);
            #else
                    // SHOW BOOT LOGO
                    M_PUSH_SCENE(_INITIAL_SCENE_FULL_NAME, false);
                    M_PUSH_SCENE(__BOOT, true);
                    ReplacePresentStatus(GameStatus::PLAYING);
            #endif
        #endif
    #else
            static_assert(false, "Please set a path for _RESOURCE_SETTING_FILE_PATH);
    #endif
#else
        // SHOW BOOT LOGO
        // GOTO SAMPLE GAME
        M_PUSH_SCENE(builtin::sample::__BOOT, true);
		ReplacePresentStatus(GameStatus::PLAYING);
#endif
	}
}

void MApplication::InitiateFonts() const {
	/** First we need initiate default font. */
	auto& font = manager::MFontManager::Instance();
    font.GenerateFont("Sans");
    font.GenerateFont("Solomon");
    font.GenerateFont("Menus");
    font.GenerateFont("BIOS");
	font.LoadDefaultFont();
}

void MApplication::InitiateDebugUi() {
#if defined(_OPGS16_DEBUG_OPTION)
	m_debug_ui_canvas = std::make_unique<CanvasDebug>();
#endif
}

void MApplication::InitiatePostProcessingEffects() {
	m_pp_manager = &manager::MPostProcessingManager::GetInstance();

	m_pp_manager->InsertEffectInitiate<shading::PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectGray>("Gray");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectSinewave>("SineWave");

	/** Set sample sequence */
	auto const result = m_pp_manager->SetSequence(1u, { "Convex" });
	if (result == nullptr) {
		std::cerr << "ERROR::CANNOT::CREATED::PP::SEQUENCE" << std::endl;
	}
}

void MApplication::SetOnBeforeUpdateCallback(const std::function<void(void)> callback) {
    m_on_before_update_callback = callback;
}

void MApplication::Run() {
    while (!glfwWindowShouldClose(m_window)) {
        m_time_manager.Update();         /*! Time ticking */
        if (m_time_manager.Ticked()) {
            m_timer_manager.Update();    /*! Timer alarm event checking */
            Update();
            Draw();
        }
    }
    /*! Must terminate glfw window */
    glfwTerminate();
}

void MApplication::Update() {
    if (m_on_before_update_callback) {  /*! If callback is bound, call function once. */
        m_on_before_update_callback();
        m_on_before_update_callback = nullptr;
    }

    Input();                            /*! Input */
    switch (GetPresentStatus()) {       /*! Update */
    case GameStatus::PLAYING:
    case GameStatus::MENU:
        if (!m_scene_manager.Empty()) {
            /*! pre-work such as Delete object, Replace object etc. */
            m_object_manager.Update();

            /*! Update */
            m_scene_manager.PresentScene()->Update();
            m_physics_manager.Update();
        }
        break;
    }

#if defined(_OPGS16_DEBUG_OPTION)
	if (IsSwitchOn(m_setting->DebugMode())) m_debug_ui_canvas->Update();
#endif

	if (IsSwitchOn(m_setting->PostProcessing()))
        m_pp_manager->UpdateSequences(); // Update active effects.
}

void MApplication::Input() {
	m_input_manager.Update();
	switch (GetPresentStatus()) {
    default: /*! Do nothing */ break;
	case GameStatus::PLAYING: InputGlobal(); break;
	case GameStatus::MENU: break;
	}
}

void MApplication::InputGlobal() {
	if (m_input_manager.IsKeyPressed("GlobalCancel"))
		PopStatus();
    if (IsSwitchOn(m_setting->SizeScalable())) {
        if (m_input_manager.IsKeyPressed("GlobalF1"))
        	ChangeScalingOption(EScaleType::X1);
        else if (m_input_manager.IsKeyPressed("GlobalF2"))
        	ChangeScalingOption(EScaleType::X2);
        else if (m_input_manager.IsKeyPressed("GlobalF3"))
        	ChangeScalingOption(EScaleType::X3);
    }

#if defined(_OPGS16_DEBUG_OPTION)
    if (m_input_manager.IsKeyPressed("GlobalF9"))
        ToggleFpsDisplay();
#endif

    if (m_input_manager.IsKeyPressed("GlobalF10"))
        TogglePostProcessingEffect();
}

void MApplication::Draw() const {
    /*! If there is no scene, do not rendering anything. */
	if (!m_scene_manager.Empty()) {
        glViewport(0, 0, SGlobalSetting::ScreenWidth(), SGlobalSetting::ScreenHeight());

		if (IsSwitchOn(m_setting->PostProcessing()))
			m_pp_manager->BindSequence(1);
        else
            m_pp_manager->BindSequence(0);

		/** Actual Rendering */
        m_scene_manager.PresentScene()->Draw();
        m_object_manager.Render();
		/** Post-processing */
        m_pp_manager->Render();
	}

#if defined(_OPGS16_DEBUG_OPTION)
	if (IsSwitchOn(m_setting->DebugMode())) m_debug_ui_canvas->Draw();
#endif

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void MApplication::ToggleFpsDisplay() const {
    m_setting->ToggleDebugMode();
	std::cerr << static_cast<bool>(m_setting->DebugMode()) << std::endl;
}

void MApplication::TogglePostProcessingEffect() const {
    m_setting->TogglePostProcessing();
	std::cerr << "POST::PROCESSING::SWITCH::" <<
        static_cast<bool>(m_setting->PostProcessing()) << std::endl;
}

void MApplication::ChangeScalingOption(EScaleType value) const {
	if (!IsSameValue(value, m_setting->ScaleValue())) {
        auto [width, height] = SGlobalSetting::ScreenSize();

		switch (value) {
		case EScaleType::X1: glfwSetWindowSize(m_window, width, height); break;
		case EScaleType::X2: glfwSetWindowSize(m_window, width << 1, height << 1); break;
		case EScaleType::X3: glfwSetWindowSize(m_window, width * 3, height * 3); break;
		}

		m_setting->SetScaleValue(value);
	}
	else {
		std::cerr << "NOTIFY::M_SCALE::VALUE::ARE::SAME" << std::endl;
	}
}

void MApplication::Exit() {
    PushStatus(GameStatus::EXIT);
    glfwSetWindowShouldClose(m_window, true);
    ReplacePresentStatus(GameStatus::TERMINATE);
}

MApplication::GameStatus MApplication::GetPresentStatus() {
    return m_game_status.top();
}

void MApplication::ReplacePresentStatus(GameStatus status) {
    m_game_status.pop();
    m_game_status.push(status);
}

void MApplication::PushStatus(GameStatus status) {
    m_game_status.push(status);
}

void MApplication::PopStatus() {
    if (!m_game_status.empty()) {
        m_game_status.pop();
        if (m_game_status.empty())
            Exit();
    }
}

MApplication::~MApplication() = default;

}