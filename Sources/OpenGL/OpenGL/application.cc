#include "application.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <glm\glm.hpp>

#include "GlobalObjects\Canvas\text.h"
#include "Scenes\start.h"
#include "System\font_manager.h"
#include "System\Shader\shader_manager.h"
#include "System\Shader\PostProcessing\pp_convex.h"
#include "System\Shader\PostProcessing\pp_sinewave.h"
#include "System\sound_manager.h"

Application::Application(std::string&& app_name)
    : window{ InitApplication(std::move(app_name)) } {
	PushStatus(GameStatus::INIT);
    // Set Camera Cursor and Fps
    camera::SetCursor(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    SetFps(60.0f);

	InitiateFonts();
	InitiateDebugUi();
	InitiatePostProcessingEffects();
	InitiateSoundSetting();

	/** Insert first scene */
    PushScene<Start>();
	ReplacePresentStatus(GameStatus::PLAYING);
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
    glfwSetCursorPosCallback(window, camera::MouseControl);

    glewInit();
    return window;
}

void Application::InitiateFonts() {
	/** First we need initiate default font. */
	auto& font = FontManager::GetInstance();
	font.InitiateFont( "Sans", "Resources/LSANS.TTF" , true);
	font.InitiateFont( "Solomon", "Resources/SolomonP.ttf" , false);
	font.LoadDefaultFont();
}

void Application::InitiateDebugUi() {
	/** Set up canvas for global information */
	auto canvas = std::make_unique<Canvas::Canvas>();
	Canvas::Text&& fps{ "", glm::vec3{32, -32, 0}, glm::vec3{0, 1, 0} }; {
		fps.SetFontSize(16);
		fps.SetOrigin(IOriginable::Origin::UP_LEFT);
		fps.SetFont("Solomon");
		canvas->InitiateChild("Fps", std::move(fps));
	}
	m_debug_ui_canvas = std::move(canvas);
}

void Application::InitiatePostProcessingEffects() {
	m_pp_manager->InsertEffectInitiate<shading::PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectSinewave>("SineWave");

	/** Set sample sequence */
	auto const result = m_pp_manager->SetSequence(0, { "SineWave", "Convex" });
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

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::Run() {
    new_time = old_time = (float)glfwGetTime();
	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        if (IfFrameTurned()) {
            Input(window);
            Update();
            Draw();
        }
    }
}

void Application::Input(GLFWwindow* const window) {
	if (DoesKeyPressed(window, GLFW_KEY_ESCAPE))
		PopStatus();
    else if (DoesKeyPressed(window, GLFW_KEY_1)) // MSAAx4
        ToggleAntialiasing();
    else if (DoesKeyPressed(window, GLFW_KEY_2)) // FPS display on/off
        ToggleFpsDisplay();
    else {
		if (GetPresentStatus() == GameStatus::PLAYING) { /** Playing */
			// Handle window has keycode into highest priority scene.
			top_scene->HandleInput(window);
		}
    }
}


void Application::ToggleFpsDisplay() {
    debug_toggled = (debug_toggled ? false : true);
}

void Application::Update() {
    if (!m_scenes.empty()) top_scene->Update();
	if (debug_toggled) UpdateDebugInformation();

	/** Temporary */ {
		if (post_processing_convex_toggled) {
			auto& pp = m_pp_manager->GetEffect("SineWave");
			pp->ReplaceUniformValue("uMove", static_cast<float>(glfwGetTime()));
		}
	}
}

void Application::UpdateDebugInformation() {
	/** Refresh Fps */ {
		std::ostringstream str;
		str << std::setprecision(4) << fps_tick;

		auto text = std::static_pointer_cast<Canvas::Text>(m_debug_ui_canvas->GetChild("Fps"));
		text->SetText("Fps : " + str.str());
	}

	/** Refresh Date */ {

	}

	m_debug_ui_canvas->Update();
}

/**
 * @brief The method calls scene to draw all objects.
 */
void Application::Draw() {
	if (!m_scenes.empty()) {
		if (post_processing_convex_toggled) { m_pp_manager->BindSequence(0); }
		/** Actual rendering */
		top_scene->Draw();
		/** Post-processing */
		if (post_processing_convex_toggled) { m_pp_manager->RenderSequence(); }
	}

	/** Debug Display */
	if (debug_toggled) DrawDebugInformation();
	/** End */
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::DrawDebugInformation() {
	m_debug_ui_canvas->Draw();
}

void Application::PopScene() {
    top_scene = nullptr;
    m_scenes.pop();
	if (!m_scenes.empty()) top_scene = m_scenes.top(); else Exit();
}

void Application::ToggleAntialiasing() {
    if (aa_toggled) {
        glDisable(GL_MULTISAMPLE);
        aa_toggled = false;
    }
    else {
        glEnable(GL_MULTISAMPLE);
        aa_toggled = true;
    }

#ifdef _DEBUG
    std::cout << "MSAA : " << (aa_toggled ? "ON" : "OFF") << std::endl;
#endif
}

bool Application::IfFrameTurned() {
    new_time = (float)glfwGetTime();
    elapsed_time += new_time - old_time;
    old_time = new_time;

    bool flag = false;
    if (elapsed_time >= interval_time) {
        fps_tick = 1 / elapsed_time;
        elapsed_time -= interval_time;
        flag = true;
    }
    return flag;
}

void Application::SetFps(float hz) {
    interval_time = 1 / hz;
}
