#include "application.h"

#include <iomanip>	// std::setprecision, set::setw, std::put_time
#include <iostream>	// std::cerr, std::endl
#include <memory>	// std::static_pointer_cast

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "GlobalObjects\Canvas\canvas.h"
#include "Objects\Debug\debug_canvas.h"     /*! CanvasDebug */
#include "Scenes\start.h"
#include "System\Shader\pp_manager.h"
#include "System\Shader\shader_manager.h"
#include "System\Shader\PostProcessing\pp_convex.h"
#include "System\Shader\PostProcessing\pp_sinewave.h"
#include "System\Shader\PostProcessing\pp_gray.h"
#include "System\Manager\font_manager.h"
#include "System\Manager\input_manager.h"
#include "System\Manager\sound_manager.h"
#include "System\Manager\time_manager.h"

Application::Application(std::string&& app_name)
    : window{ InitApplication(std::move(app_name)) },
	m_option{ false, true, false, true },
	m_scale{ OptionScale::X1_DEFAULT } {
	PushStatus(GameStatus::INIT);
}

GLFWwindow* Application::InitApplication(std::string&& app_name) {
    glfwInit();

    // OpenGL Setting
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Set MSAAx4
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, app_name.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    std::cerr << "Hello world. App name : " + app_name << std::endl;

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetFramebufferSizeCallback(window, &Application::FramebufferSizeCallback);
    //glfwSetCursorPosCallback(window, camera::MouseControl);

    glewInit();
    return window;
}

const std::array<unsigned, 2> Application::GetDefaultScreenSize() const {
	return std::array<unsigned, 2>{SCREEN_WIDTH, SCREEN_HEIGHT};
}

Scene* const Application::GetTopScene() const noexcept {
    if (!m_scenes.empty())
        return m_scenes.top().get();
    else
        return nullptr;
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::Initiate() {
	if (GetPresentStatus() == GameStatus::INIT) {
		// Set Camera Cursor and Fps
		//camera::SetCursor(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
        m_m_time = &TimeManager::GetInstance();
        m_m_time->SetFps(60.f);

		InitiateFonts();
		InitiateDebugUi();
		InitiatePostProcessingEffects();
		InitiateSoundSetting();
		m_m_input = &InputManager::GetInstance();
        m_m_input->Initialize(window);

		/** Insert first scene */
		PushScene<Start>();
		ReplacePresentStatus(GameStatus::PLAYING);

        glEnable(GL_DEPTH_TEST);
	}
}

void Application::InitiateFonts() {
	/** First we need initiate default font. */
	auto& font = FontManager::GetInstance();
	font.InitiateFont( "Sans", "Resources/arial.ttf" , true);
	font.InitiateFont( "Solomon", "Resources/SolomonP.ttf" , false);
	font.InitiateFont( "Menus", "Resources/Fonts/Menus.ttf" , false);	/** Recommend 9pt */
	font.LoadDefaultFont();
}

void Application::InitiateDebugUi() {
	m_debug_ui_canvas = std::make_unique<CanvasDebug>();
}

void Application::InitiatePostProcessingEffects() {
	m_pp_manager = &shading::PostProcessingManager::GetInstance();

	m_pp_manager->InsertEffectInitiate<shading::PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectGray>("Gray");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectSinewave>("SineWave");

	/** Set sample sequence */
	auto id = 0u;
	auto const result = m_pp_manager->SetSequence(id, { "SineWave", "SineWave" });
	if (result == nullptr) {
		std::cerr << "ERROR::CANNOT::CREATED::PP::SEQUENCE" << std::endl;
	}
}

void Application::InitiateSoundSetting() {
	auto& manager = SoundManager::GetInstance();

	using SoundType = SoundManager::SoundType;
	using FileType = SoundManager::FileType;
	manager.InsertSound("Music1", "Resources/sample.wav", SoundType::BACKGROUND, FileType::WAV);
	manager.PlaySound("Music1");
}

void Application::Run() {
    while (!glfwWindowShouldClose(window)) {
        m_m_time->Update();
        if (m_m_time->Ticked()) {
            Update();
            Draw();
        }
    }

    glfwTerminate();
}

void Application::Update() {
    /*! Input */
    Input();
    /*! Update */
    if (!m_scenes.empty()) top_scene->Update();
	if (m_option.debug_mode)
        m_debug_ui_canvas->Update();
	m_pp_manager->UpdateSequences(); // Update active effects.
}

void Application::Input() {
	m_m_input->Update();
	switch (GetPresentStatus()) {
	case GameStatus::PLAYING: InputGlobal(); break;
	case GameStatus::MENU: break;
	}
}

void Application::InputGlobal() {
	if (m_m_input->IsKeyPressed("GlobalCancel"))
		PopStatus();
	if (m_option.size_scalable) {
		if (m_m_input->IsKeyPressed("GlobalF1"))
			ChangeScalingOption(OptionScale::X1_DEFAULT);
		else if (m_m_input->IsKeyPressed("GlobalF2"))
			ChangeScalingOption(OptionScale::X2_DOUBLE);
		else if (m_m_input->IsKeyPressed("GlobalF3"))
			ChangeScalingOption(OptionScale::X3_TRIPLE);
		else if (m_m_input->IsKeyPressed("GlobalF9"))
			ToggleFpsDisplay();
		else if (m_m_input->IsKeyPressed("GlobalF10"))
			TogglePostProcessingEffect();
	}
}

/**
 * @brief The method calls scene to draw all objects.
 */
void Application::Draw() {
	if (!m_scenes.empty()) {
		if (m_option.post_processing) {
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			m_pp_manager->BindSequence(0);
		}

		/** Actual rendering */
		top_scene->Draw();

		/** Post-processing */
		if (m_option.post_processing) {
			m_pp_manager->RenderSequence();
			glViewport(0, 0,
					   SCREEN_WIDTH * static_cast<int>(m_scale),
					   SCREEN_HEIGHT * static_cast<int>(m_scale));
			m_pp_manager->Render();
		}
	}

	if (m_option.debug_mode)
        m_debug_ui_canvas->Draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::ToggleFpsDisplay() {
    m_option.debug_mode = (m_option.debug_mode ? false : true);
	std::cerr << m_option.debug_mode << std::endl;
}

void Application::TogglePostProcessingEffect() {
	m_option.post_processing = (m_option.post_processing ? false : true);
	std::cerr << "NOTIFY::POST::PROCESSING::SWITCH::" << m_option.post_processing << std::endl;
}

void Application::PopScene() {
    top_scene = nullptr;
    m_scenes.pop();
    if (!m_scenes.empty()) top_scene = m_scenes.top(); else Exit();
}

void Application::ChangeScalingOption(OptionScale value) {
	if (m_scale != value) {
		switch (value) {
		case OptionScale::X1_DEFAULT:
			glfwSetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;
		case OptionScale::X2_DOUBLE:
			glfwSetWindowSize(window, SCREEN_WIDTH << 1, SCREEN_HEIGHT << 1);
			break;
		case OptionScale::X3_TRIPLE:
			glfwSetWindowSize(window, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3);
			break;
		}
		m_scale = value;
	}
	else {
		std::cerr << "NOTIFY::M_SCALE::VALUE::ARE::SAME" << std::endl;
	}
}

void Application::ToggleAntialiasing() {
    if (m_option.anti_aliasing) {
        glDisable(GL_MULTISAMPLE);
        m_option.anti_aliasing = false;
    }
    else {
        glEnable(GL_MULTISAMPLE);
        m_option.anti_aliasing = true;
    }

#ifdef _DEBUG
    std::cout << "MSAA : " << (m_option.anti_aliasing ? "ON" : "OFF") << std::endl;
#endif
}


void Application::Exit() {
    glfwSetWindowShouldClose(window, true);
}