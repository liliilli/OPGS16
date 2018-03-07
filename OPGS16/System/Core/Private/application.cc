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

#include "../Public/application.h"              /*! Header file */

#include <iostream>	                            /*! std::cerr, std::endl */
#include <string_view>                          /*! std::string_view */
#include "../Public/core_header.h"              /*! Subsequential header files */
#include "../Public/core_setting.h"

namespace opgs16 {

namespace {

using namespace std::string_view_literals;
using opgs16::debug::PushLog;
using opgs16::debug::MsgType;

constexpr std::string_view g_global_resource_path{ "_resource.meta"sv };

/*! Callback method for size check and resizing */
void OnCallbackFrameBufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

}

Application::Application() :
    m_logger{ &debug::Logger::operator(), std::ref(debug::Logger::Instance()) },
    m_window{ InitApplication(u8"OPGS16") },
    m_setting_manager{ manager::SettingManager::Instance() },
    m_input_manager{ manager::InputManager::Instance() },
    m_object_manager{ manager::ObjectManager::Instance() },
    m_physics_manager{ manager::PhysicsManager::Instance() },
    m_resource_manager{ manager::ResourceManager::Instance() },
    m_scene_manager{ manager::SceneManager::Instance() },
    m_sound_manager{ manager::SoundManager::Instance() },
    m_time_manager{ manager::TimeManager::Instance() },
    m_timer_manager{ manager::TimerManager::Instance() } {

    m_setting = std::make_unique<GlobalSetting>();
	PushStatus(GameStatus::INIT);
}

GLFWwindow* Application::InitApplication(const std::string& app_name) const {
    glfwInit();

    /*! OpenGL Setting */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    PushLog(MsgType::OK, "GLFW CONTEXT VERSION 4.3 Core.");

    /*! Set MSAAx4 */
    //glfwWindowHint(GLFW_SAMPLES, 4);

    const auto window = glfwCreateWindow(GlobalSetting::ScreenWidth(),
                                         GlobalSetting::ScreenHeight(),
                                         app_name.c_str(),
                                         nullptr, nullptr);
    if (!window) {
        PushLog(MsgType::ERROR, "Failed to create GLFW window. Application will terminate.");
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

void Application::Initiate() {
	if (GetPresentStatus() == GameStatus::INIT) {
        /*! Initialize Global Setting. */
        m_resource_manager.ReadResourceFile(g_global_resource_path.data());

        /*! Initialize resource list. */
        m_time_manager.SetFps(60.f);
        m_input_manager.Initialize(m_window);
        m_sound_manager.ProcessInitialSetting();

		InitiateFonts();
		InitiateDebugUi();
		InitiatePostProcessingEffects();

        /*! Set gl rendering options. */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

		/** Insert first scene */
        m_resource_manager.ReadResourceFile(R"(_Project/Maintenance/_meta/_resource.meta)");
        m_scene_manager.PushScene<Maintenance>();
		ReplacePresentStatus(GameStatus::PLAYING);
	}
}

void Application::InitiateFonts() const {
	/** First we need initiate default font. */
	auto& font = manager::FontManager::Instance();
    font.GenerateFont("Sans");
    font.GenerateFont("Solomon");
    font.GenerateFont("Menus");
    font.GenerateFont("BIOS");
	font.LoadDefaultFont();
}

void Application::InitiateDebugUi() {
	m_debug_ui_canvas = std::make_unique<CanvasDebug>();
}

void Application::InitiatePostProcessingEffects() {
	m_pp_manager = &manager::PostProcessingManager::GetInstance();

	m_pp_manager->InsertEffectInitiate<shading::PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectGray>("Gray");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectSinewave>("SineWave");

	/** Set sample sequence */
	auto const result = m_pp_manager->SetSequence(1u, { "Convex" });
	if (result == nullptr) {
		std::cerr << "ERROR::CANNOT::CREATED::PP::SEQUENCE" << std::endl;
	}
}

void Application::SetOnBeforeUpdateCallback(const std::function<void(void)> callback) {
    m_on_before_update_callback = callback;
}

void Application::Run() {
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
    if (m_logger.joinable()) {
        debug::Logger::Instance().Terminate();
        m_logger.join();
    }
}

void Application::Update() {
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

	if (IsSwitchOn(m_setting->DebugMode()))
        m_debug_ui_canvas->Update();
	if (IsSwitchOn(m_setting->PostProcessing()))
        m_pp_manager->UpdateSequences(); // Update active effects.
}

void Application::Input() {
	m_input_manager.Update();
	switch (GetPresentStatus()) {
    default: /*! Do nothing */ break;
	case GameStatus::PLAYING: InputGlobal(); break;
	case GameStatus::MENU: break;
	}
}

void Application::InputGlobal() {
	if (m_input_manager.IsKeyPressed("GlobalCancel"))
		PopStatus();
    if (IsSwitchOn(m_setting->SizeScalable())) {
        if (m_input_manager.IsKeyPressed("GlobalF1"))
        	ChangeScalingOption(ScaleType::X1);
        else if (m_input_manager.IsKeyPressed("GlobalF2"))
        	ChangeScalingOption(ScaleType::X2);
        else if (m_input_manager.IsKeyPressed("GlobalF3"))
        	ChangeScalingOption(ScaleType::X3);
    }

    if (m_input_manager.IsKeyPressed("GlobalF9"))
        ToggleFpsDisplay();
    if (m_input_manager.IsKeyPressed("GlobalF10"))
        TogglePostProcessingEffect();
}

void Application::Draw() const {
    /*! If there is no scene, do not rendering anything. */
	if (!m_scene_manager.Empty()) {
        glViewport(0, 0, GlobalSetting::ScreenWidth(), GlobalSetting::ScreenHeight());

		if (IsSwitchOn(m_setting->PostProcessing()))
			m_pp_manager->BindSequence(1);
        else
            m_pp_manager->BindSequence(0);

		/** Actual Rendering */
        m_object_manager.Render();
        //m_scene_manager.PresentScene()->Draw();
		/** Post-processing */
        m_pp_manager->Render();
	}

	if (IsSwitchOn(m_setting->DebugMode())) m_debug_ui_canvas->Draw();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Application::ToggleFpsDisplay() const {
    m_setting->ToggleDebugMode();
	std::cerr << static_cast<bool>(m_setting->DebugMode()) << std::endl;
}

void Application::TogglePostProcessingEffect() const {
    m_setting->TogglePostProcessing();
	std::cerr << "POST::PROCESSING::SWITCH::" <<
        static_cast<bool>(m_setting->PostProcessing()) << std::endl;
}

void Application::ChangeScalingOption(ScaleType value) const {
	if (!IsSameValue(value, m_setting->ScaleValue())) {
        auto [width, height] = GlobalSetting::ScreenSize();

		switch (value) {
		case ScaleType::X1: glfwSetWindowSize(m_window, width, height); break;
		case ScaleType::X2: glfwSetWindowSize(m_window, width << 1, height << 1); break;
		case ScaleType::X3: glfwSetWindowSize(m_window, width * 3, height * 3); break;
		}

		m_setting->SetScaleValue(value);
	}
	else {
		std::cerr << "NOTIFY::M_SCALE::VALUE::ARE::SAME" << std::endl;
	}
}

void Application::Exit() {
    PushStatus(GameStatus::EXIT);
    glfwSetWindowShouldClose(m_window, true);
    ReplacePresentStatus(GameStatus::TERMINATE);
}

Application::GameStatus Application::GetPresentStatus() {
    return m_game_status.top();
}

void Application::ReplacePresentStatus(GameStatus status) {
    m_game_status.pop();
    m_game_status.push(status);
}

void Application::PushStatus(GameStatus status) {
    m_game_status.push(status);
}

void Application::PopStatus() {
    if (!m_game_status.empty()) {
        m_game_status.pop();
        if (m_game_status.empty())
            Exit();
    }
}

Application::~Application() = default;

}