
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/application.cc
///
/// @brief
/// Implementation file of Core/application.h
///
/// @log
/// 2018-04-14 Add file information comment and Removed unused header file.
/// 2018-05-19 Change singleton structure to namespace structure.
///

#include <Core\application.h>   /// Header file

#include <Manager\font_manager.h>
#include <Manager\input_manager.h>
#include <Manager\object_manager.h>
#include <Manager\physics_manager.h>
#include <Manager\postprocessing_manager.h>
#include <Manager\prerendering_manager.h>
#include <Manager\resource_manager.h>
#include <Manager\scene_manager.h>
#include <Manager\setting_manager.h>
#include <Manager\shader_manager.h>
#include <Manager\sound_manager.h>
#include <Manager\time_manager.h>
#include <Manager\timer_manager.h>

#include <Shader\PostProcessing\pp_convex.h>
#include <Shader\PostProcessing\pp_sinewave.h>
#include <Shader\PostProcessing\pp_gray.h>

/// ::opgs16 core setting file
#include <Core\core_setting.h>
/// ::opgs16::entry::_internal::EGameStatus
#include <Core\Internal\application_status.h>
/// ::opgs16::entry::_internal strong enum boolean flags.
#include <Core\Internal\application_flag.h>

/// expanded assertion
#include <Helper\assert.h>
/// import logger
#include <Headers\import_logger.h>

// @todo Adjust each project manifest file path not to write .. chars.

#include <..\manifest.h>        /// ::opgs16::manifest

#include <Debug\debug_canvas.h>
/// opgs16::manifest::sample::boot
#include <Core\Boot\__boot.h>

#if defined(_INITIAL_SCENE_INCLUDE_PATH)
#include _INITIAL_SCENE_INCLUDE_PATH
#endif

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Static intergrity checking
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

static_assert(opgs16::manifest::k_size <= 3,
    "manifest k_size must be range from 1 to 3.");
static_assert(opgs16::manifest::k_size > 0,
    "manifest k_size must be range from 1 to 3.");

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::entry::_internal::EInitiated;
using opgs16::entry::_internal::EOperated;

EInitiated m_initiated  = EInitiated::NotInitiated;
EOperated m_operated    = EOperated::NotOperated;
} /// unnamed namespace

namespace {

constexpr float k_fps_count = 60.f;

// Window handle pointer
GLFWwindow* m_window = nullptr;

opgs16::manager::MPostProcessingManager* m_pp_manager = nullptr;
#if defined(false)
opgs16::manager::MObjectManager* m_object_manager = nullptr;
#endif
opgs16::manager::MPhysicsManager* m_physics_manager = nullptr;
opgs16::manager::MSceneManager* m_scene_manager = nullptr;
opgs16::manager::MSoundManager* m_sound_manager = nullptr;
opgs16::manager::MTimeManager* m_time_manager = nullptr;
opgs16::manager::MTimerManager* m_timer_manager = nullptr;

std::stack<opgs16::entry::_internal::EGameStatus> m_game_status;

std::unique_ptr<opgs16::SGlobalSetting> m_setting = nullptr;

// This callback will be called before update routine only once.
std::function<void(void)> m_on_before_update_callback = nullptr;

#if defined(_OPGS16_DEBUG_OPTION)
// Debug UI components container
std::unique_ptr<opgs16::element::CObject> m_debug_ui_canvas = nullptr;
#endif

///
/// @brief
/// Callback method for size check and resizing.
///
void OnCallbackFrameBufferSize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

} /// unnamed namespace

namespace opgs16::entry {

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward declaration part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*


GLFWwindow* InitApplication(const std::string& application_name);


void PushStatus(_internal::EGameStatus status);


void PopStatus();

///
/// @brief
/// Initiate and Make font informations.
///
void InitiateDefaultFonts();

///
/// @brief
/// Initiate post-processing effects in advance.
///
void InitiatePostProcessingEffects();

///
/// @brief
/// The method update components movement, UI refresh, and so on.
///
void Update();

///
/// @deprecated
/// @todo Remove due to confusion of OPGS16 script input check system.
///
void Input();

///
/// @brief
/// The method calls scene to draw all m_object_list.
///
void Draw();

///
///
void ChangeScalingOption(EScaleType value);

void InputGlobal();

///
/// @brief
/// Return present status.
/// @return GameStatus
/// value on top of stack, m_global_game_status saves game status.
///
_internal::EGameStatus GetPresentStatus();

///
/// @brief
/// Replace present status to the other status.
/// @param[in]
/// status New status value to replace present status with.
///
void ReplacePresentStatus(_internal::EGameStatus status);

void ToggleFpsDisplay();
void TogglePostProcessingEffect();
void ToggleCollisionBoxDisplay();

#if defined(_OPGS16_DEBUG_OPTION)
///
/// @brief
/// Initiate and Compose Debug Interface components.
///
void InitiateDebugUi();
#endif

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Implementation part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

void SetOnBeforeUpdateCallback(const std::function<void(void)> callback) {
    m_on_before_update_callback = callback;
}

SGlobalSetting& Setting() noexcept {
  return *m_setting;
}

void Initiate() {
  NEU_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated function call of ::opgs16::entry::Initiate() is prohibited.");
  m_initiated = EInitiated::Initiated;

#if defined(_OPGS16_DEBUG_OPTION)
  m_window = InitApplication("OPGS16 DEBUG MODE");
#else
#if defined (_CUSTOM_PROJECT)
#if !defined (_APPLICATION_PROJECT_NAME)
  static_assert(false,
      "Application project name is not active."
      "Please uncomment or make macro _APPLICATION_PROJECT_NAME");
#endif
#if !(_APPLICATION_PROJECT_NAME + 0)
  static_assert(false,
      "Application project name is not valid. Check manifest file.");
#endif
#if defined (_APPLICATION_WINDOW_NAME)
#if !(_APPLICATION_WINDOW_NAME + 0)
  static_assert(false,
      "Application window name is not valid, check manifest file.");
#else
  m_window{ InitApplication(_APPLICATION_WINDOW_NAME) },
#endif
#else
  m_window{ InitApplication(_APPLICATION_PROJECT_NAME) },
#endif
#else
  static_assert(false,
      "Please turn on _CUSTOM_PROJECT macro to make window properly.");
#endif
#endif

  manager::setting::Initiate();
  manager::input::Initiate(m_window);
  manager::resource::ReadResourceFile("_resource.meta");
  manager::object::Initiate();
  manager::font::Initiate();

  m_scene_manager = &manager::MSceneManager::Instance();
  m_sound_manager = &manager::MSoundManager::Instance();
  m_time_manager = &manager::MTimeManager::Instance();
  m_timer_manager = &manager::MTimerManager::Instance();

  m_setting = std::make_unique<SGlobalSetting>();
  PushStatus(_internal::EGameStatus::INIT);

  // Initialize Global Setting.

  // Initialize resource list.
  m_time_manager->SetFps(k_fps_count);
  m_sound_manager->ProcessInitialSetting();

  InitiateDefaultFonts();
  InitiatePostProcessingEffects();
#if defined(_OPGS16_DEBUG_OPTION)
  InitiateDebugUi();
#endif

  /*! Set gl rendering options. */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);

  if constexpr (manifest::k_size == 1)
    ChangeScalingOption(EScaleType::X1);
  else if constexpr (manifest::k_size == 2)
    ChangeScalingOption(EScaleType::X2);
  else
    ChangeScalingOption(EScaleType::X3);

#if defined(_CUSTOM_PROJECT)
#if defined(_RESOURCE_SETTING_FILE_PATH)
  manager::resource::ReadResourceFile(_RESOURCE_SETTING_FILE_PATH);
#else
  static_assert(false, "Set a path for _RESOURCE_SETTING_FILE_PATH);
#endif

#if defined(_RESOURCE_SETTING_FILE_PATH)
#if defined(_INITIAL_SCENE_FULL_NAME)
#if !_SHOW_BOOT_SCREEN
  M_PUSH_SCENE(_INITIAL_SCENE_FULL_NAME, true);
  ReplacePresentStatus(_internal::EGameStatus::PLAYING);
#else
  // SHOW BOOT LOGO
  M_PUSH_SCENE(_INITIAL_SCENE_FULL_NAME, false);
  M_PUSH_SCENE(builtin::sample::__BOOT, true);
  ReplacePresentStatus(GameStatus::PLAYING);
#endif
#endif
#else
  static_assert(false, "Set a path for _RESOURCE_SETTING_FILE_PATH);
#endif
#else
  // SHOW BOOT LOGO
  // GOTO SAMPLE GAME
  m_resource_manager.ReadResourceFile(L"System/Boot/Resources/_resource.meta");

  M_PUSH_SCENE(builtin::sample::__BOOT, true);
  ReplacePresentStatus(GameStatus::PLAYING);
#endif

}

///
/// @brief The method that initiates application.
/// Initiate glfw, glew, create window and return window object pointer.
///
/// @param[in] app_name Application title name.
/// @return Window handle pointer.
///
GLFWwindow* InitApplication(const std::string& application_name) {
  glfwInit();

  // OpenGL Setting
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  PUSH_LOG_DEBUG("GLFW CONTEXT VERSION 4.3 Core.");

  const auto window = glfwCreateWindow(SGlobalSetting::ScreenWidth(),
                                       SGlobalSetting::ScreenHeight(),
                                       application_name.c_str(),
                                       nullptr, nullptr);
  if (!window) {
    PUSH_LOG_ERRO("Failed to create GLFW window. Application will terminate.");
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
  glfwSetFramebufferSizeCallback(window, &OnCallbackFrameBufferSize);
  //glfwSetCursorPosCallback(window, camera::MouseControl);

  glewInit();
  return window;
}

///
/// @brief
/// Pile new status up game status stack.
/// Every routines refers to game status stack will look up new status.
///
/// @param[in] status New status value to pile up onto status stack.
///
void PushStatus(_internal::EGameStatus status) {
  m_game_status.push(status);
}

///
/// @brief
/// Pop(Halt) present status and return to previous status.
/// If there is no more status in stack, exit application automatically.
///
/// Do not use this method when replace one value with the other status value,
/// This will crash game on playing.
///
void PopStatus() {
  if (!m_game_status.empty()) {
    m_game_status.pop();
    if (m_game_status.empty()) {
      PushStatus(_internal::EGameStatus::EXIT);
      glfwSetWindowShouldClose(m_window, true);
      ReplacePresentStatus(_internal::EGameStatus::TERMINATE);
    }
  }
}

void InitiateDefaultFonts() {
  manager::font::GenerateFont("Sans");
  manager::font::GenerateFont("Solomon");
  manager::font::GenerateFont("Menus");
  manager::font::GenerateFont("BIOS");
  manager::font::GenerateFont("Sam3");
  manager::font::GenerateFont("Pixel");
}

#if defined(_OPGS16_DEBUG_OPTION)
void InitiateDebugUi() {
	m_debug_ui_canvas = std::make_unique<CanvasDebug>();
}
#endif

void InitiatePostProcessingEffects() {
	m_pp_manager = &manager::MPostProcessingManager::GetInstance();

  using builtin::postprocessing::PpEffectConvex;
  using builtin::postprocessing::PpEffectGray;
  using builtin::postprocessing::PpEffectSinewave;
	m_pp_manager->InsertEffectInitiate<PpEffectConvex>("Convex");
	m_pp_manager->InsertEffectInitiate<PpEffectGray>("Gray");
	m_pp_manager->InsertEffectInitiate<PpEffectSinewave>("SineWave");

	// Set sample sequence
  // @todo : Renovate postprocessing manager sequence setting mechanism.
	auto const result = m_pp_manager->SetSequence(1u, { "Convex" });
	if (result == nullptr) {
		std::cerr << "ERROR::CANNOT::CREATED::PP::SEQUENCE" << std::endl;
	}
}

void Run() {
  NEU_ASSERT(m_operated == EOperated::NotOperated,
      "Duplicated function call of ::opgs16::entry::Run() is prohibited.");
  m_operated = EOperated::Operated;

  while (!glfwWindowShouldClose(m_window)) {
    // Check time ticking following given frame per second.
    m_time_manager->Update();

    if (m_time_manager->Ticked()) {
      // Timer alarm event checking.
      m_timer_manager->Update();

      Update();
      Draw();
    }
  }

  // Must terminate glfw window
  glfwTerminate();
}

void Update() {
  // If callback is being bound,
  // call function once and terminate callback function.
  if (m_on_before_update_callback) {
    m_on_before_update_callback();
    m_on_before_update_callback = nullptr;
  }

  // Pre-processing (Pre-rendering) update.
  manager::prerendering::Update();

  Input();

  switch (GetPresentStatus()) {
  case _internal::EGameStatus::PLAYING:
  case _internal::EGameStatus::MENU:
    if (!m_scene_manager->Empty()) {
      // pre-work such as Delete object, Replace object etc.
      manager::object::Update();

      // Update
      m_scene_manager->PresentScene()->Update();
      manager::physics::Update();
    }
    break;
  default: break;;
  }

#if defined(_OPGS16_DEBUG_OPTION)
  if (IsSwitchOn(m_setting->DebugMode()))
    m_debug_ui_canvas->Update();
  if (IsSwitchOn(m_setting->CollisionAABBBoxDisplay()))
    manager::physics::RenderCollisionBox();
#endif

  // Update active effects.
  if (IsSwitchOn(m_setting->PostProcessing()))
    m_pp_manager->UpdateSequences();
}

void Input() {
  manager::input::Update();

  switch (GetPresentStatus()) {
  case _internal::EGameStatus::PLAYING:
    InputGlobal();
    break;
  default: break;
  }
}

void InputGlobal() {
  using manager::input::IsKeyPressed;
	if (IsKeyPressed("GlobalCancel"))
		PopStatus();

  if (IsSwitchOn(m_setting->SizeScalable())) {
    if (IsKeyPressed("GlobalF1"))
      ChangeScalingOption(EScaleType::X1);
    else if (IsKeyPressed("GlobalF2"))
      ChangeScalingOption(EScaleType::X2);
    else if (IsKeyPressed("GlobalF3"))
      ChangeScalingOption(EScaleType::X3);
  }

  // @todo : Renovate debug mode input and detach it from release mode.

#if defined(_OPGS16_DEBUG_OPTION)
  if (IsKeyPressed("GlobalF7"))
    ToggleCollisionBoxDisplay();
  if (IsKeyPressed("GlobalF9"))
    ToggleFpsDisplay();
#endif

  if (IsKeyPressed("GlobalF10"))
    TogglePostProcessingEffect();
}

void Draw() {
  // If there is no scene, do not rendering anything.
  if (!m_scene_manager->Empty()) {
    glViewport(0, 0,
               SGlobalSetting::ScreenWidth(),
               SGlobalSetting::ScreenHeight());

    // Pre-processing (Pre-rendering) render
    manager::prerendering::Render();

    // Actual Rendering
    if (IsSwitchOn(m_setting->PostProcessing()))
      m_pp_manager->BindSequence(1);
    else
      m_pp_manager->BindSequence(0);

    m_scene_manager->PresentScene()->Draw();
    manager::object::Render();

    if (m_setting->CollisionAABBBoxDisplay() == Switch::ON) {
      manager::object::RenderAABB();
    }

    // Postprocessing
    m_pp_manager->Render();
  }

#if defined(_OPGS16_DEBUG_OPTION)
  if (IsSwitchOn(m_setting->DebugMode()))
    m_debug_ui_canvas->Draw();
#endif

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void ToggleFpsDisplay() {
  m_setting->ToggleDebugMode();
	std::cerr << static_cast<bool>(m_setting->DebugMode()) << std::endl;
}

void TogglePostProcessingEffect() {
  m_setting->TogglePostProcessing();
	std::cerr << "POST::PROCESSING::SWITCH::" <<
        static_cast<bool>(m_setting->PostProcessing()) << std::endl;
}

void ToggleCollisionBoxDisplay() {
  m_setting->ToggleCollisionAABBBoxDisplay();
  PUSH_LOG_INFO_EXT("Toggle Collision box display {}",
      (m_setting->CollisionAABBBoxDisplay() == Switch::ON) ? "ON" : "OFF");
}

  void ChangeScalingOption(EScaleType value) {
	if (!IsSameValue(value, m_setting->ScaleValue())) {
    auto [width, height] = SGlobalSetting::ScreenSize();

		switch (value) {
		case EScaleType::X1:
		  glfwSetWindowSize(m_window, width, height);
		  break;
		case EScaleType::X2:
		  glfwSetWindowSize(m_window, width << 1, height << 1);
		  break;
		case EScaleType::X3:
		  glfwSetWindowSize(m_window, width * 3, height * 3);
		  break;
		}

		m_setting->SetScaleValue(value);
	}
	else {
		std::cerr << "NOTIFY::M_SCALE::VALUE::ARE::SAME" << std::endl;
	}
}

_internal::EGameStatus GetPresentStatus() {
  return m_game_status.top();
}

void ReplacePresentStatus(_internal::EGameStatus status) {
  m_game_status.pop();
  m_game_status.push(status);
}

} /// ::opgs16::entry