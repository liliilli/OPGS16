#include "application.h"

#include <iostream>	// std::cerr, std::endl

#include <GL\glew.h>
#include <GLFW\glfw3.h>

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
#include "System\Manager\object_manager.h"  /*! ObjectManager */
#include "System\Manager\physics_manager.h" /*! PhysicsManager*/
#include "System\Manager\resource_manager.h"/*! ResourceManager */
#include "System\Manager\scene_manager.h"   /*! SceneManager */
#include "System\Manager\setting_manager.h" /*! SettingManager */
#include "System\Manager\sound_manager.h"
#include "System\Manager\time_manager.h"

Application::Application(std::string&& app_name)
    : window{ InitApplication(std::move(app_name)) },
    m_scene_instance{ SceneManager::GetInstance() },
	m_option{ false, true, true, true },
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

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::Initiate() {
	if (GetPresentStatus() == GameStatus::INIT) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ResourceManager::GetInstance();	// Initialize resource list.
        SettingManager::GetInstance();  // Initialize

        m_m_time = &TimeManager::GetInstance();
        m_m_time->SetFps(60.f);

		InitiateFonts();
		InitiateDebugUi();
		InitiatePostProcessingEffects();
		InitiateSoundSetting();
		m_m_input = &InputManager::GetInstance();
        m_m_input->Initialize(window);

        m_physics_manager = &PhysicsManager::GetInstance();
        m_object_manager = &ObjectManager::GetInstance();

		/** Insert first scene */
        m_scene_instance.PushScene<Start>();

        glEnable(GL_DEPTH_TEST);
		ReplacePresentStatus(GameStatus::PLAYING);
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
	auto const result = m_pp_manager->SetSequence(1u, { "Convex" });
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
    switch (m_game_status.top()) {
    case GameStatus::PLAYING: //[[fallthrough]] require /std:c++17
    case GameStatus::MENU:
        if (!m_scene_instance.SceneEmpty()) {
            /*! pre-work such as Delete object, Replace object etc. */
            m_object_manager->Update();

            /*! Update */
            m_scene_instance.GetPresentScene()->Update();
            m_physics_manager->Update();
        }
        break;
    }

    if (m_option.debug_mode)
        m_debug_ui_canvas->Update();
	if (m_option.post_processing)
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
    /*! If there is no scene, do not rendering anything. */
	if (!m_scene_instance.SceneEmpty()) {
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (m_option.post_processing)
			m_pp_manager->BindSequence(1);
        else
            m_pp_manager->BindSequence(0);

		/** Actual Rendering */
        m_scene_instance.GetPresentScene()->Draw();
		/** Post-processing */
        m_pp_manager->Render();
	}

	if (m_option.debug_mode) m_debug_ui_canvas->Draw();

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
    PushStatus(GameStatus::EXIT);
    glfwSetWindowShouldClose(window, true);
    ReplacePresentStatus(GameStatus::TERMINATE);
}