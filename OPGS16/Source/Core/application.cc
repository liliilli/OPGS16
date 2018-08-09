#include <precompiled.h>
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

/// Header file
#include <Core/application.h>

#include <stack>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Phitos/Dbg/assert.h>

#include <Manager/font_manager.h>
#include <Manager/input_manager.h>
#include <Manager/mesh_manager.h>
#include <Manager/object_manager.h>
#include <Manager/physics_manager.h>
#include <Manager/postprocessing_manager.h>
#include <Manager/prerendering_manager.h>
#include <Manager/resource_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/setting_manager.h>
#include <Manager/shader_manager.h>
#include <Manager/sound_manager.h>
#include <Manager/time_manager.h>
#include <Manager/timer_manager.h>
#include <Manager/Internal/vao_management.h>

#include <Shader/PostProcessing/pp_convex.h>
#include <Shader/PostProcessing/pp_sinewave.h>
#include <Shader/PostProcessing/pp_gray.h>

/// ::opgs16 core setting file
#include <Core/core_setting.h>
/// ::opgs16::entry::_internal::EGameStatus
#include <Core/Internal/application_status.h>
/// ::opgs16::entry::_internal strong enum boolean flags.
#include <Core/Internal/application_flag.h>
/// import logger
#include <Headers/import_logger.h>

// @todo Adjust each project manifest file path not to write .. chars.

/// ::opgs16::manifest
#include <../manifest.h>

#include <Debug/debug_canvas.h>
/// opgs16::manifest::sample::boot
#include <Core/Boot/__boot.h>

#if defined(OP16_SETTING_FIRST_SCENE_INCLUDE_RELATIVE_PATH)
#include OP16_SETTING_FIRST_SCENE_INCLUDE_RELATIVE_PATH
#endif

///
/// Static intergrity checking
///

#if defined(OP16_SETTING_START_SCALE_X1)
  #if defined(OP16_SETTING_START_SCALE_X2) ||\
      defined(OP16_SETTING_START_SCALE_X3)
    static_assert(false, "Just one of scaling values must be turned on.");
  #endif
#elif defined(OP16_SETTING_START_SCALE_X2)
  #if defined(OP16_SETTING_START_SCALE_X1) ||\
      defined(OP16_SETTING_START_SCALE_X3)
    static_assert(false, "Just one of scaling values must be turned on.");
  #elif defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "640 x 480 resolution does not support scaling. turn on X1 macro.");
  #endif
#elif defined(OP16_SETTING_START_SCALE_X3)
  #if defined(OP16_SETTING_START_SCALE_X1) ||\
      defined(OP16_SETTING_START_SCALE_X2)
    static_assert(false, "Just one of scaling values must be turned on.");
  #elif defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "640 x 480 resolution does not support scaling. turn on X1 macro.");
  #endif
#else
  static_assert(false, "At least one of scaling values must be turned on.");
#endif

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

//!
//! Data
//!

namespace {

// Window handle pointer
GLFWwindow* m_window = nullptr;
std::stack<opgs16::entry::_internal::EGameStatus> m_game_status;

// This callback will be called before update routine only once.
std::function<void()> m_on_before_update_callback = nullptr;

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

void Shutdown() {
  glfwTerminate();

  using namespace opgs16;
  manager::postprocessing::__::Shutdown();
  manager::timer::ClearAllTimers();
  manager::sound::ReleaseAllSoundElements();
  manager::sound::__::Shutdown();
  manager::scene::__::Shutdown();
  manager::font::__::Shutdown();
  manager::_internal::vao::Shutdown();
  manager::mesh::Shutdown();
  manager::resource::__::Shutdown();
}

} /// unnamed namespace

namespace opgs16::entry {

//!
//! Forward declaration list
//!

///
///
GLFWwindow* InitApplication(const std::string& application_name);

///
///
void PushStatus(_internal::EGameStatus status);

///
///
void PopStatus();

///
/// @brief
/// Initiate post-processing effects in advance.
///
void InitiatePostProcessingEffects();

///
/// @brief
/// The method update components movement, UI refresh, and so on.
///
/// @param[in] delta_time Delta time of each frame.
///
void Update(float delta_time);

///
/// @brief
/// The method calls scene to draw all m_object_list.
///
void Draw();

///
///
void ChangeScalingOption(EScaleType value);

///
///
///
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

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated function call of ::opgs16::entry::Initiate() is prohibited.");
  m_initiated = EInitiated::Initiated;

#if defined(_OPGS16_DEBUG_OPTION)
  m_window = InitApplication("OPGS16 DEBUG MODE");
#else
  #if !defined (OP16_SETTING_APPLICATION_NAME)
    static_assert(false,
        "Application project name is not active."
        "Please uncomment or make macro OP16_SETTING_APPLICATION_NAME");
  #endif

  #if defined (OP16_SETTING_APPLICATION_WINDOW_NAME)
    m_window = InitApplication(OP16_SETTING_APPLICATION_WINDOW_NAME);
  #else
    m_window = InitApplication(OP16_SETTING_APPLICATION_NAME);
  #endif
#endif

  manager::setting::Initiate();
  manager::resource::__::Initiate();

  manager::mesh::Initiate();
  manager::_internal::vao::Initiate();

  manager::shader::Initiate();
  manager::object::Initiate();
  manager::font::__::Initiate();
  manager::postprocessing::__::Initiate();

  manager::sound::__::Initiate();
  manager::input::Initiate(m_window);

  PushStatus(_internal::EGameStatus::INIT);

  manager::time::SetFps(static_cast<float>(setting::GetFixedFpsValue()));
  manager::time::SetVsync(phitos::enums::ESwitch::Off);

  // Initialize resource list.
  InitiatePostProcessingEffects();

  // Set gl rendering options.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);

#if defined(OP16_SETTING_START_SCALE_X1)
  ChangeScalingOption(EScaleType::X1);
#elif defined(OP16_SETTING_START_SCALE_X2)
  ChangeScalingOption(EScaleType::X2);
#elif defined(OP16_SETTING_START_SCALE_X3)
  ChangeScalingOption(EScaleType::X3);
#endif

#if defined(_OPGS16_DEBUG_OPTION)
  InitiateDebugUi();
#endif

#if defined(OP16_SETTING_FIRST_SCENE_FULL_NAME)
#if !defined(OP16_SETTING_BOOTSCREEN_SHOW)
  M_PUSH_SCENE(OP16_SETTING_FIRST_SCENE_FULL_NAME, true);
  ReplacePresentStatus(_internal::EGameStatus::PLAYING);
#else
  // SHOW BOOT SCREEN
  M_PUSH_SCENE(OP16_SETTING_FIRST_SCENE_FULL_NAME, false);
  M_PUSH_SCENE(builtin::sample::__BOOT, true);
  ReplacePresentStatus(_internal::EGameStatus::PLAYING);
#endif
#endif
}

///
/// @brief The method that initiates application.
/// Initiate glfw, glew, create window and return window object pointer.
///
/// @param[in] application_name Application title name.
/// @return Window handle pointer.
///
GLFWwindow* InitApplication(const std::string& application_name) {
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;

  glfwInit();
  PHITOS_SET_RELEASE_FUNCTION(&Shutdown);

  // OpenGL Setting
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  PUSH_LOG_DEBUG("GLFW CONTEXT VERSION 4.3 Core.");

  const auto window = glfwCreateWindow(GetScreenWidth(),
                                       GetScreenHeight(),
                                       application_name.c_str(),
                                       nullptr, nullptr);
  if (!window) {
    PUSH_LOG_ERRO("Failed to create GLFW window. Application will terminate.");
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
  glfwSetFramebufferSizeCallback(window, &OnCallbackFrameBufferSize);
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
      glfwSetWindowShouldClose(m_window, true);
    }
  }
}

#if defined(_OPGS16_DEBUG_OPTION)
void InitiateDebugUi() {
	m_debug_ui_canvas = std::make_unique<CanvasDebug>();
}
#endif

void InitiatePostProcessingEffects() {
  using phitos::enums::ESucceed;
  using builtin::postprocessing::PpEffectConvex;
  using builtin::postprocessing::PpEffectGray;
  using builtin::postprocessing::PpEffectSinewave;
  using manager::postprocessing::InsertEffectInitiate;

	InsertEffectInitiate<PpEffectConvex>("Convex");
	InsertEffectInitiate<PpEffectGray>("Gray");
	InsertEffectInitiate<PpEffectSinewave>("SineWave");

	// Set sample sequence
	auto [ptr, result] = manager::postprocessing::SetSequence("opConvex", { "Convex" });
	if (result == ESucceed::Failed)
    PUSH_LOG_INFO("Failed to create post-processing sequence.");
}

void Run() {
  PHITOS_ASSERT(m_operated == EOperated::NotOperated,
      "Duplicated function call of ::opgs16::entry::Run() is prohibited.");
  m_operated = EOperated::Operated;

  while (!glfwWindowShouldClose(m_window)) {
    // Check time ticking following given frame per second.
    manager::time::Update();

    if (manager::time::Ticked()) {
      // Timer alarm event checking.
      const float dt = manager::time::GetDeltaTime();
      manager::timer::Update(dt);
      if (m_game_status.empty()) break;

      Update(dt);
      if (m_game_status.empty()) break;

      Draw();
    }
  }

  Shutdown();
}

void ExitGame() {
  while (!m_game_status.empty()) {
    PopStatus();
  }
}

void Update(float delta_time) {
  using opgs16::setting::IsEnablePostProcessing;

  /// If callback is being bound,
  /// call function once and terminate callback function.
  if (m_on_before_update_callback) {
    m_on_before_update_callback();
    m_on_before_update_callback = nullptr;
  }

  /// Pre-processing (Pre-rendering) update.
  manager::prerendering::Update();
  manager::input::Update();

  switch (GetPresentStatus()) {
  case _internal::EGameStatus::PLAYING:
#ifdef _OPGS16_DEBUG_OPTION
    InputGlobal();
#endif
    if (!manager::scene::IsSceneEmpty()) {
      /// pre-work such as Delete object, Replace object etc.
      manager::object::Update(delta_time);
      /// Update
      manager::scene::GetPresentScene()->Update(delta_time);
      manager::physics::Update(delta_time);
    }
    break;
  default: break;
  }

#if defined(_OPGS16_DEBUG_OPTION)
  if (IsSwitchOn(m_setting->DebugMode()))
    m_debug_ui_canvas->Update(delta_time);
  if (IsSwitchOn(m_setting->CollisionAABBBoxDisplay()))
    manager::physics::RenderCollisionBox();
#endif

  /// Update active effects.
  if (IsEnablePostProcessing())
    manager::postprocessing::UpdateSequences();
}

void InputGlobal() {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::setting::IsEnableScaling;

	if (IsKeyPressed("GlobalCancel"))
		PopStatus();

  if (IsEnableScaling()) {
    if (IsKeyPressed("GlobalF1"))
      ChangeScalingOption(EScaleType::X1);
    else if (IsKeyPressed("GlobalF2"))
      ChangeScalingOption(EScaleType::X2);
    else if (IsKeyPressed("GlobalF3"))
      ChangeScalingOption(EScaleType::X3);
  }

  if (IsKeyPressed("GlobalF7"))
    ToggleCollisionBoxDisplay();
  if (IsKeyPressed("GlobalF9"))
    ToggleFpsDisplay();

  if (IsKeyPressed("GlobalF10"))
    TogglePostProcessingEffect();
}

void Draw() {
  using opgs16::setting::GetScreenHeight;
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::IsEnablePostProcessing;
  using opgs16::setting::IsEnableRenderingAabb;

  // If there is no scene, do not rendering anything.
  if (!manager::scene::IsSceneEmpty()) {
    glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

    // Pre-processing (Pre-rendering) render
    manager::prerendering::Render();

    // Actual Rendering
    if (IsEnablePostProcessing())
      manager::postprocessing::BindSequence("opConvex");
    else
      manager::postprocessing::BindSequence("opDefault");

    manager::scene::GetPresentScene()->Draw();
    manager::object::Render();

    if (IsEnableRenderingAabb()) {
      manager::object::RenderAABB();
    }

    manager::postprocessing::Render();
  }

#if defined(_OPGS16_DEBUG_OPTION)
  if (IsSwitchOn(m_setting->DebugMode()))
    m_debug_ui_canvas->Draw();
#endif

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void ToggleFpsDisplay() {
  setting::ToggleDebugMode();
}

void TogglePostProcessingEffect() {
  setting::TogglePostProcessing();
}

void ToggleCollisionBoxDisplay() {
  using opgs16::setting::IsEnableRenderingAabb;
  using opgs16::setting::ToggleCollisionAABBBoxDisplay;

  ToggleCollisionAABBBoxDisplay();
  PUSH_LOG_INFO_EXT(
      "Toggle Collision box display {}",
      (IsEnableRenderingAabb() ? "ON" : "OFF")
  );
}

void ChangeScalingOption(EScaleType value) {
#if !defined(OP16_SETTING_RESOLUTION_640480)
  using opgs16::setting::GetScaleValue;
  using opgs16::setting::GetScreenSize;
  using opgs16::setting::SetScaleValue;

	if (value != GetScaleValue()) {
    auto& [width, height] = GetScreenSize();

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

		SetScaleValue(value);
	}
#endif
}

_internal::EGameStatus GetPresentStatus() {
  return m_game_status.top();
}

void ReplacePresentStatus(_internal::EGameStatus status) {
  m_game_status.pop();
  m_game_status.push(status);
}

} /// ::opgs16::entry