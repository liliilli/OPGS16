#include "application.h"

#include <chrono>	// std::chrono::system_clock
#include <ctime>	// std::time_t, std::localtime, std::put_time
#include <iomanip>	// std::setprecision, set::setw, std::put_time
#include <iostream>	// std::cerr, std::endl
#include <memory>	// std::static_pointer_cast
#include <string>	// std::stringstream, std::string

#include <glm\glm.hpp>

#include "GlobalObjects\Canvas\text.h"
#include "Scenes\start.h"
#include "System\font_manager.h"
#include "System\Shader\shader_manager.h"
#include "System\Shader\PostProcessing\pp_convex.h"
#include "System\Shader\PostProcessing\pp_sinewave.h"
#include "System\Shader\PostProcessing\pp_gray.h"
#include "System\sound_manager.h"

Application::Application(std::string&& app_name)
    : window{ InitApplication(std::move(app_name)) },
	m_aa_toggled{ false },
	m_debug_toggled{ true },
	m_post_processing_toggled{ true },
	m_is_size_scalable{ true },
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
    glfwSetCursorPosCallback(window, camera::MouseControl);

    glewInit();
    return window;
}

void Application::Initiate() {
	if (GetPresentStatus() == GameStatus::INIT) {
		// Set Camera Cursor and Fps
		//camera::SetCursor(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		SetFps(60.0f);

		InitiateFonts();
		InitiateDebugUi();
		InitiatePostProcessingEffects();
		InitiateSoundSetting();

		/** Insert first scene */
		PushScene<Start>();
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
	/** Set up canvas for global information */
	auto canvas = std::make_unique<Canvas::Canvas>();
	glm::vec3 colors = glm::vec3{ 1, 0, 1 };
	Canvas::Text&& fps{ "", glm::vec3{16, -16, 0}, colors }; {
		fps.SetFontSize(8);
		fps.SetOrigin(IOriginable::Origin::UP_LEFT);
		fps.SetFont("Solomon");
		canvas->InitiateChild("Fps", std::move(fps));
	}
	Canvas::Text&& date{ "", glm::vec3{16, -24, 0}, colors }; {
		date.SetFontSize(8);
		date.SetOrigin(IOriginable::Origin::UP_LEFT);
		date.SetFont("Solomon");
		canvas->InitiateChild("Date", std::move(date));
	}
	Canvas::Text&& hier{ "", glm::vec3{16, -32, 0}, colors }; {
		hier.SetFontSize(8);
		hier.SetOrigin(IOriginable::Origin::UP_LEFT);
		hier.SetFont("Solomon");
		canvas->InitiateChild("Hier", std::move(hier));
	}

	m_debug_ui_canvas = std::move(canvas);
}

void Application::InitiatePostProcessingEffects() {
	m_pp_manager = &shading::PostProcessingManager::GetInstance();

	m_pp_manager->InsertEffectInitiate<shading::PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectGray>("Gray");
	m_pp_manager->InsertEffectInitiate<shading::PpEffectSinewave>("SineWave");

	/** Set sample sequence */
	auto id = 0u;
	auto const result = m_pp_manager->SetSequence(id, { "Gray", "SineWave", "Convex" });
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

std::array<unsigned, 2> Application::GetDefaultScreenSize() {
	return std::array<unsigned, 2>{SCREEN_WIDTH, SCREEN_HEIGHT};
}

void Application::Input(GLFWwindow* const window) {
	if (DoesKeyPressed(window, GLFW_KEY_ESCAPE))
		PopStatus();
	else if (DoesKeyPressed(window, GLFW_KEY_1) && m_is_size_scalable) // MSAAx4
		ChangeScalingOption(OptionScale::X1_DEFAULT);
	else if (DoesKeyPressed(window, GLFW_KEY_2) && m_is_size_scalable) // FPS display on/off
		ChangeScalingOption(OptionScale::X2_DOUBLE);
	else if (DoesKeyPressed(window, GLFW_KEY_3) && m_is_size_scalable)
		ChangeScalingOption(OptionScale::X3_TRIPLE);
	else if (DoesKeyPressed(window, GLFW_KEY_0)) {
		//ToggleAntialiasing();
		ToggleFpsDisplay();
	}
	else if (DoesKeyPressed(window, GLFW_KEY_9))
		TogglePostProcessingEffect();
    else {
		if (GetPresentStatus() == GameStatus::PLAYING) { /** Playing */
			// Handle window has keycode into highest priority scene.
			top_scene->HandleInput(window);
		}
    }
}

void Application::Update() {
    if (!m_scenes.empty()) top_scene->Update();
	if (m_debug_toggled) UpdateDebugInformation();
	m_pp_manager->UpdateSequences(); // Update active effects.
}

/**
 * @brief The method calls scene to draw all objects.
 */
void Application::Draw() {
	if (!m_scenes.empty()) {
		if (m_post_processing_toggled) {
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			m_pp_manager->BindSequence(0);
		}

		/** Actual rendering */
		top_scene->Draw();

		/** Post-processing */
		if (m_post_processing_toggled) {
			m_pp_manager->RenderSequence();
			glViewport(0, 0,
					   SCREEN_WIDTH * static_cast<int>(m_scale),
					   SCREEN_HEIGHT * static_cast<int>(m_scale));
			m_pp_manager->Render();
		}
	}

	if (m_debug_toggled) DrawDebugInformation(); // Debug Display
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::ToggleFpsDisplay() {
    m_debug_toggled = (m_debug_toggled ? false : true);
	std::cerr << m_debug_toggled << std::endl;
}

void Application::TogglePostProcessingEffect() {
	m_post_processing_toggled = (m_post_processing_toggled ? false : true);
	std::cerr << "NOTIFY::POST::PROCESSING::SWITCH::" << m_post_processing_toggled << std::endl;
}

void Application::UpdateDebugInformation() {
	/** Refresh Fps */ {
		std::ostringstream str;
		str << std::setprecision(4) << fps_tick;

		auto text = std::static_pointer_cast<Canvas::Text>(m_debug_ui_canvas->GetChild("Fps"));
		text->SetText("Fps : " + str.str());
	}

	/** Refresh Date */ {
		auto timepoint = std::chrono::system_clock::now();
		std::time_t time_struct = std::chrono::system_clock::to_time_t(timepoint);
		std::ostringstream stream;
		stream << std::put_time(std::localtime(&time_struct), "%F %T");

		auto date = std::static_pointer_cast<Canvas::Text>(m_debug_ui_canvas->GetChild("Date"));
		date->SetText(stream.str());
	}

	/** Display Hierarchy Objects */ {
		ObjectTree tree{};
		if (!m_scenes.empty()) {
			top_scene->GetObjectTree(&tree);
			std::string text{};
			SetHierarchyText(&tree, 0, &text);

			auto tree = std::static_pointer_cast<Canvas::Text>(m_debug_ui_canvas->GetChild("Hier"));
			tree->SetText(std::move(text));
		}
	}

	m_debug_ui_canvas->Update();
}

void Application::SetHierarchyText(const ObjectTree* item, size_t count, std::string* const text) {
	if (count == 0)
		text->append("Scene\n");
	else {
		std::string space_text{};
		for (auto i = 1; i <= count; ++i) { space_text.push_back(' '); }
		text->append(space_text + item->name + '\n');
	}

	for (const auto& child : item->children) {
		SetHierarchyText(&child, count + 1, text);
	}
}

void Application::DrawDebugInformation() {
	m_debug_ui_canvas->Draw();
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
    if (m_aa_toggled) {
        glDisable(GL_MULTISAMPLE);
        m_aa_toggled = false;
    }
    else {
        glEnable(GL_MULTISAMPLE);
        m_aa_toggled = true;
    }

#ifdef _DEBUG
    std::cout << "MSAA : " << (m_aa_toggled ? "ON" : "OFF") << std::endl;
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
