#include "application.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <al.h>
#include <alc.h>
#include <AL\alut.h>

#include <glm\glm.hpp>

#include "GlobalObjects\Canvas\text.h"
#include "Scenes\start.h"
#include "System\font_manager.h"
#include "System\Shader\shader_manager.h"

Application::Application(std::string&& app_name)
    : window{ InitApplication(std::move(app_name)) } {
    // Set Camera Cursor and Fps
    camera::SetCursor(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    SetFps(60.0f);

	InitiateFonts();
	InitiateDebugUi();
	InitiatePostProcessingEffects();
	InitiateSoundSetting();

	/** Insert first scene */
    PushScene<Start>();
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
	m_canvas = std::move(canvas);
}

void Application::InitiatePostProcessingEffects() {
	m_pp_manager->InsertEffect("Convex");
	m_pp_manager->GetEffect("Convex")->Initiate();
}

void Application::InitiateSoundSetting() {
	/** Find device automatically */
	ALCdevice* device{ nullptr };
	device = alcOpenDevice(nullptr);
	if (!device) {
		std::cerr << "ERROR::NOT::FOUND::SOUND::DEVICE" << std::endl;
	}
	/** In order to render audio scene, create and initialize a context */
	ALCcontext* sound_context{ nullptr };
	sound_context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(sound_context) && !CheckSoundError()) {
		std::cerr << "ERROR::FAILED::TO::CREATE::SOUND::CONTEXT" << std::endl;
	}
	/** Must create the source which is actually the origin of sound. */
	ALuint source;
	alGenSources(1, &source);
	//if (!CheckSoundError());
	alSourcef(source, AL_PITCH, 1);
	//if (!CheckSoundError());
	//alSourcei(source, AL_LOOPING, AL_TRUE);
	//if (!CheckSoundError());

	/** Must create the buffer which stores stream of sound from source */
	ALuint buffer;
	alGenBuffers(1, &buffer);
	//if (!CheckSoundError());

	/** And load sound stream to buffer */
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;

	alutLoadWAVFile((ALbyte*)"Resources/sample.wav", &format, &data, &size, &freq, &loop);
	CheckSoundError();
	alBufferData(buffer, format, data, size, freq);

	/** Bind source to buffer, in order to actually output sound. */
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
}

bool Application::CheckSoundError() {
	ALCenum sound_error = alGetError();
	if (sound_error != AL_NO_ERROR) {
		std::cerr << "ERROR::SOUND::AL::SOMETHING::HAPPENDED" << std::endl;
		return false;
	}
	return true;
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::Run() {
    new_time = old_time = (float)glfwGetTime();
	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        if (IfFrameTurned()) {
            ProcessInput(window);
            Update();
            Draw();
        }
    }
}

void Application::ProcessInput(GLFWwindow* const window) {
    if (DoesKeyPressed(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    else if (DoesKeyPressed(window, GLFW_KEY_1)) // MSAAx4
        ToggleAntialiasing();
    else if (DoesKeyPressed(window, GLFW_KEY_2)) // FPS display on/off
        ToggleFpsDisplay();
    else {
        // Handle window has keycode into highest priority scene.
        top_scene->HandleInput(window);
    }
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

void Application::ToggleFpsDisplay() {
    fps_toggled = (fps_toggled ? false : true);
}

void Application::Update() {
    top_scene->Update();
	UpdateDebugInformation();
}

void Application::UpdateDebugInformation() {
	if (fps_toggled) {
		m_canvas->Update();
	}
}

void Application::Draw() {
	if (post_processing_convex_toggled) {
		m_pp_manager->GetEffect("Convex")->Bind();
	}

	top_scene->Draw();
	DrawDebugInformation();

	if (post_processing_convex_toggled) {
		m_pp_manager->GetEffect("Convex")->RenderEffect();
	}

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::DrawDebugInformation() {
    if (fps_toggled) { // If fps display toggled, draw fps.
        std::ostringstream str;
        str << std::setprecision(4) << display_time;

		auto text = std::static_pointer_cast<Canvas::Text>(m_canvas->GetChild("Fps"));
		text->SetText("Fps : " + str.str());

		m_canvas->Draw();
    }
}

void Application::PopScene() {
    top_scene = nullptr;
    scenes.pop();

    if (scenes.empty()) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool Application::IfFrameTurned() {
    new_time = (float)glfwGetTime();
    elapsed_time += new_time - old_time;
    old_time = new_time;

    bool flag = false;
    if (elapsed_time >= interval_time) {
        display_time = 1 / elapsed_time;
        elapsed_time -= interval_time;
        flag = true;
    }
    return flag;
}

void Application::SetFps(float hz) {
    interval_time = 1 / hz;
}
