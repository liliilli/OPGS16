
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/input.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring.
/// 2018-05-20 Get rid of singleton pattern and rebuild it to namespace.
/// 2018-06-17 Revise own format input file to json.
///
/// @todo implement mouse click
/// @todo implement stick key , stick mouse
/// @todo implement mouse cursor graphics
/// @todo refactoring
///

/// Header file
#include <Manager/input_manager.h>

#include <fstream>
#include <string_view>
#include <unordered_map>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

/// nlohmann::json reading library
#include <json.hpp>
/// Enhanced assertion
#include <Phitos/Dbg/assert.h>
/// phitos::enums::ESuccess
#include <Phitos/Enums/success.h>

/// ::opgs16;:core::application
#include <Core\application.h>
/// ::opgs16::SGlobalSetting
#include <Core\core_setting.h>

/// Import logger
#include <Headers/import_logger.h>
/// ::opgs16::helper::json
#include <Helper/Json/json_helper.h>
/// ::opgs16::helper string helper function.
#include <Helper/string_helper.h>

/// Header file
#include <Manager/time_manager.h>
/// ::opgs16::debug error messages.
#include <Manager\Internal\error_message.h>
/// ::opgs16::manager::_internal flags
#include <Manager\Internal\flag.h>
/// ::opgs16::manager::_internal::BindingKeyInfo
#include <Manager\Internal\input_internal.h>

/// ::manifest
#include <../manifest.h>

///
/// @enum EKeyExist
/// @brief
///
enum class EKeyExist : bool {
  NotExist = false,
  Exist = true
};

//!
//! Forward declaration
//!

///
/// @brief
///
void ReadInputFile(const std::string& file_path);

///
/// @brief
/// Let each key value where key status is KeyInputStatus::RELEASED
/// falling down into dead_zone and change status into KeyInputStatus::NEUTRAL
/// along with neutral_gravity.
///
/// This methods gets delta time from Application time data,
/// multiply it with gravity and fall it down to 0 (neutral value).
///
/// @param[in] key_info Key information to apply.
///
void ProceedGravity(opgs16::manager::_internal::BindingKeyInfo& key_info);

///
/// @brief
///
/// @param[in] key
///
EKeyExist IsKeyExist(const std::string& key);

///
/// @brief verify and automatically log present status.
///
/// @param[in] json Json loading library instance
/// @param[in] key Keyword to find.
/// @param[in] file_path File path of json library.
/// @param[in] swt Switch of keyword.
///
void ModeVerifyKey(const nlohmann::json& json,
                   const std::string& key,
                   const std::string& file_path, phitos::enums::ESwitch& swt);

///
/// @brief
///
/// @param[in] key
/// @param[in] key_value
///
phitos::enums::ESucceed KeyboardVerifyKey(const std::string& key,
                                          const nlohmann::json& key_value);

///
/// @brief
///
/// @param[in] atlas_json
///
/// @return
///
phitos::enums::ESucceed BindKeyboardKeyInformation(const nlohmann::json& atlas_json);

///
/// @brief
///
/// @param[in] it
///
/// @return
///
phitos::enums::ESucceed KeyboardBindKey(
    const nlohmann::basic_json<>::const_iterator& it);

//!
//! Data
//!

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;

EInitiated m_initiated = EInitiated::NotInitiated;

} /// unnamed namespace

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {
using opgs16::manager::_internal::BindingKeyInfo;
using opgs16::manager::input::EKeyPrimaryState;
using namespace std::string_view_literals;
using TKeyMap = std::unordered_map<std::string, BindingKeyInfo>;

// Window handle pointer
GLFWwindow* m_window;
GLFWcursor* m_cursor = nullptr;

EKeyPrimaryState m_primary_keys[349];
TKeyMap m_key_inputs;

} /// unnamed namespace

//!
//! Global function declaration
//!

EKeyExist IsKeyExist(const std::string& key) {
  return static_cast<EKeyExist>(m_key_inputs.find(key) != m_key_inputs.end());
}

///
/// @brief
/// Polling notification of physical key input.
/// If key is notified glfw library execute this function as callback.
///
/// In this now, just print what key was pressed on console.
///
/// @param[in] window GLFW window instance.
/// @param[in] key Key information.
/// @param[in] scancode Not be used now.
/// @param[in] action Key pressed, released, keeping pushed states.
/// @param[in] mod Not be used now.
///
static void __InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
  PUSH_LOG_DEBUG_EXT("Key input : {0}, {1}", key, action);

  switch (action) {
  case GLFW_PRESS:
    m_primary_keys[key] = EKeyPrimaryState::Pressed;
    break;
  case GLFW_RELEASE:
    m_primary_keys[key] = EKeyPrimaryState::Released;
    break;
  case GLFW_REPEAT:
    m_primary_keys[key] = EKeyPrimaryState::Repeated;
    break;
  default: break;
  }
}

///
/// @brief
/// Polling mouse position update.
///
/// In this now, just print how much cursor moved on window.
/// origin is left, down (0, 0). max size is (256, 224).
///
/// @param[in] window GLFW window instance.
/// @param[in] xpos x coordinate position value.
/// @param[in] ypos y coordinate position value.
///
static void __MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
  const auto& setting = opgs16::entry::Setting();
  const auto scale_value = setting.ScaleValueIntegerOf();

  const auto regulated_xpos = xpos / scale_value;
  const auto regulated_ypos = ypos / scale_value;

  PUSH_LOG_DEBUG_EXT(
      "Mouse position update : O {0:2}, {1:2}, R {2:2}, {3:2}",
      xpos, ypos, regulated_xpos, regulated_ypos);
}

///
/// @brief
/// Polling mouse input update.
///
/// In this now, just print what moust button is pressed or released.
///
/// @param[in] window GLFW window instance.
/// @param[in] button Mouse button
/// @param[in] action Mouse button action.
/// @param[in] modes Mouse modifier bits.
///
static void __MouseInputCallback(GLFWwindow* window,
    int button, int action, int modes) {
  PUSH_LOG_DEBUG_EXT(
      "Mouse input update [Button : {0}], [Action : {1}]", button, action);
}

void SetMouseCursorTemporary() {
  unsigned char m_data[16 * 16 * 4];
  memset(m_data, 0xFF, sizeof(m_data));
  GLFWimage image;
  image.height = 16;
  image.width = 16;
  image.pixels = m_data;
  m_cursor = glfwCreateCursor(&image, 0, 0);

  glfwSetCursor(m_window, m_cursor);
}

//!
//! Implementation
//!

namespace opgs16::manager::input {

void Initiate(GLFWwindow* window_context) {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      debug::err_input_duplicated_init);

  m_window = window_context;
  glfwSetKeyCallback(m_window, __InputKeyCallback);
  glfwSetCursorPosCallback(m_window, __MousePositionCallback);
  glfwSetMouseButtonCallback(m_window, __MouseInputCallback);
  SetMouseCursorTemporary();

  ReadInputFile(helper::ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH,
                                                "Setting/input.meta"));
  m_initiated = EInitiated::Initiated;
}

float GetKeyValue(const std::string& key) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated, debug::err_input_not_initiated);

  if (IsKeyExist(key) == EKeyExist::NotExist) {
    PUSH_LOG_ERROR_EXT(debug::err_input_key_not_exist, key);
		return 0.f;
	}

  return m_key_inputs[key].value;
}

bool IsKeyPressed(const std::string& key) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated, debug::err_input_not_initiated);

  if (m_key_inputs.find(key) == m_key_inputs.end()) {
    PUSH_LOG_ERROR_EXT(debug::err_input_key_not_exist, key);
		return false;
  }

  BindingKeyInfo& key_info = m_key_inputs[key];

  switch (key_info.key_status) {
  case BindingKeyInfo::KeyInputStatus::NEG_PRESSED:
  case BindingKeyInfo::KeyInputStatus::POS_PRESSED:
    if (key_info.send_signal)
      return false;

    if (!key_info.stick_key) {
      key_info.send_signal = true;
      return true;
    }
    [[fallthrough]];
  case BindingKeyInfo::KeyInputStatus::NegativeRepeated:
  case BindingKeyInfo::KeyInputStatus::PositiveRepeated:
    return true;
  default:
    return false;
  }
}

bool IsKeyReleased(const std::string& key) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated, debug::err_input_not_initiated);

  if (IsKeyExist(key) == EKeyExist::NotExist) {
    PUSH_LOG_ERROR_EXT(debug::err_input_key_not_exist, key);
    return false;
  }

  switch (m_key_inputs[key].key_status) {
  case BindingKeyInfo::KeyInputStatus::NEUTRAL:
  case BindingKeyInfo::KeyInputStatus::RELEASED:
    return true;
  default:
    return false;
  }
}

void Update() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated, debug::err_input_not_initiated);

	for (auto& key_info : m_key_inputs) {
		auto& key = key_info.second;
		// PRESSED, NEUTRAL checks key pressed event.
		// If key released in state PRESSED, change it to RELEASED.
		using Status = BindingKeyInfo::KeyInputStatus;

		switch (key.key_status) {
		case Status::RELEASED:
			ProceedGravity(key);
      [[fallthrough]];
		case Status::NEUTRAL:
			if (key.neg != GLFW_KEY_UNKNOWN &&
          m_primary_keys[key.neg] == EKeyPrimaryState::Pressed) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN &&
               m_primary_keys[key.pos] == EKeyPrimaryState::Pressed) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			break;
		case Status::NEG_PRESSED:
			if (key.pos != GLFW_KEY_UNKNOWN &&
          m_primary_keys[key.pos] == EKeyPrimaryState::Pressed) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			else if (key.neg != GLFW_KEY_UNKNOWN) {
        const auto key_md = m_primary_keys[key.neg];
        if (key.stick_key && key_md == EKeyPrimaryState::Repeated) {
          key.value = -1.0f;
          key.key_status = Status::NegativeRepeated;
        }
        else if (key_md == EKeyPrimaryState::Released) {
          key.key_status = Status::RELEASED;
          ProceedGravity(key);
        }
      }
			break;
		case Status::POS_PRESSED:
			if (key.neg != GLFW_KEY_UNKNOWN &&
          m_primary_keys[key.neg] == EKeyPrimaryState::Pressed) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN) {
        const auto key_md = m_primary_keys[key.pos];
        if (key.stick_key && key_md == EKeyPrimaryState::Repeated) {
          key.value = 1.0f;
          key.key_status = Status::PositiveRepeated;
        }
        else if (key_md == EKeyPrimaryState::Released) {
          key.key_status = Status::RELEASED;
          ProceedGravity(key);
        }
      }
			break;
    case Status::PositiveRepeated:
      if (key.neg != GLFW_KEY_UNKNOWN &&
          m_primary_keys[key.neg] == EKeyPrimaryState::Pressed) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN) {
        const auto key_md = m_primary_keys[key.pos];
        if (key_md == EKeyPrimaryState::Released) {
          key.key_status = Status::RELEASED;
          ProceedGravity(key);
        }
      }
      break;
    case Status::NegativeRepeated:
      if (key.pos != GLFW_KEY_UNKNOWN &&
          m_primary_keys[key.pos] == EKeyPrimaryState::Pressed) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			else if (key.neg != GLFW_KEY_UNKNOWN) {
        const auto key_md = m_primary_keys[key.neg];
        if (key_md == EKeyPrimaryState::Released) {
          key.key_status = Status::RELEASED;
          ProceedGravity(key);
        }
      }
      break;
		}
	}
}

} /// opgs16::manager::input

//!
//! Global function definition
//!

void ReadInputFile(const std::string& file_path) {
  std::ifstream stream { file_path, std::ios_base::in };
  if (!stream.good()) {
    PUSH_LOG_CRITICAL_EXT(opgs16::debug::err_input_failed_load_file, file_path);
    PUSH_LOG_ERRO("Input feature will be disabled.");
    stream.close();

    // @todo Do something before everything is going to be mess.
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return;
  }

  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using phitos::enums::ESwitch;
  using opgs16::helper::json::IsJsonKeyExist;

  nlohmann::json atlas_json;
  stream >> atlas_json;
  stream.close();

  if (IsJsonKeyExist(atlas_json, "mode") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT(opgs16::debug::err_input_failed_json_file, "mode", file_path);
    PUSH_LOG_ERRO("Input feature will be disabled.");
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return;
  }

  const auto input_mode = atlas_json["mode"];

  ESwitch keyboard_activated = ESwitch::On;
  ESwitch mouse_activated    = ESwitch::On;
  ESwitch joystick_activated = ESwitch::Off;

  // Joystick verification did not held, because not supported yet.
  ModeVerifyKey(input_mode, "keyboard", file_path, keyboard_activated);
  ModeVerifyKey(input_mode, "mouse", file_path, mouse_activated);

  if (keyboard_activated == ESwitch::On) {
    if (BindKeyboardKeyInformation(atlas_json) == ESucceed::Failed) {
      PUSH_LOG_WARN(opgs16::debug::err_input_failed_failed_bind_key);
    }
  }

  if (mouse_activated == ESwitch::On) {
    if (atlas_json.find("mouse") == atlas_json.end()) {
      PUSH_LOG_CRITICAL_EXT(opgs16::debug::err_input_failed_json_file, "mouse", file_path);
      PUSH_LOG_ERRO("Input feature will be disabled.");
      PHITOS_NOT_IMPLEMENTED_ASSERT();
      return;
    }
    const auto mouse = atlas_json["mouse"];
  }
}

void ModeVerifyKey(const nlohmann::json& json,
                   const std::string& key,
                   const std::string& file_path, phitos::enums::ESwitch& swt) {
  using phitos::enums::ESwitch;
  using phitos::enums::EFound;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, "keyboard") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT(opgs16::debug::err_input_keyboard_not_exist, key, file_path);
    PUSH_LOG_ERROR_EXT(opgs16::debug::err_input_disable_msg, "keyboard");
    swt= ESwitch::Off;
  }

  if (json.count("keyboard") != 1) {
    PUSH_LOG_CRITICAL_EXT("Duplicated {} is found. [{} : {}]",
                          key, "Path", file_path);
    PUSH_LOG_ERROR_EXT(opgs16::debug::err_input_disable_msg, "keyboard");
    swt = ESwitch::Off;
  }

  PUSH_LOG_DEBUG_EXT("Mode key {} is {}", key, swt == ESwitch::On ? "ON": "OFF");
}

phitos::enums::ESucceed KeyboardVerifyKey(const std::string& key,
                                          const nlohmann::json& key_value) {
  using phitos::enums::ESucceed;
  using phitos::enums::EFound;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(key_value, "+") == EFound::NotFound &&
      IsJsonKeyExist(key_value, "-") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Keyboard key {} does not have any key binding.", key);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(key_value, "gravity") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Keyboard key {} does not have gravity.", key);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(key_value, "stick") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Keyboard key {} does not have stick.", key);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

phitos::enums::ESucceed BindKeyboardKeyInformation(const nlohmann::json& atlas_json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(atlas_json, "keyboard") == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file.", "keyboard");
    PUSH_LOG_ERRO("Input feature will be disabled.");

    // @todo fix this.
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return ESucceed::Failed;
  }
  const auto keyboard = atlas_json["keyboard"];

  for (auto it = keyboard.begin(); it != keyboard.end(); ++it) {
    const std::string key = it.key();
    const auto& value = it.value();

    if (IsKeyExist(key) == EKeyExist::Exist) {
      PUSH_LOG_ERROR_EXT(
          "Keyboard key {} is duplicated. "
          "key {} will not be performed properly.", key);
      continue;
    }

    if (KeyboardVerifyKey(key, value) == ESucceed::Failed) {
      PUSH_LOG_ERROR_EXT(
          "Failed to verify keyboard key {0}. "
          "Keyboard key {0} will not bind to input system.", key);
      continue;
    }

    if (KeyboardBindKey(it) == ESucceed::Failed) {
      PUSH_LOG_ERROR_EXT("Failed to bind keyboard key {}.", key);
      return ESucceed::Failed;
    }
  }

  return ESucceed::Succeed;
}

phitos::enums::ESucceed KeyboardBindKey(const nlohmann::basic_json<>::const_iterator& it) {
  using phitos::enums::ESucceed;

  const auto key    = it.key();
  const auto& value = it.value();
  BindingKeyInfo key_information;

  if (auto pos_it = value.find("+"); pos_it != value.end()) {
    const auto& pos_it_value = pos_it.value();

    if (!pos_it_value.is_number_unsigned()) {
      PUSH_LOG_ERROR_EXT("Keyboard key {} {} value is not number.", key, "positive");
      return ESucceed::Failed;
    }
    key_information.pos = pos_it_value.get<unsigned>();
  }

  if (auto neg_it = value.find("-"); neg_it != value.end()) {
    const auto& neg_it_value = neg_it.value();

    if (!neg_it_value.is_number_unsigned()) {
      PUSH_LOG_ERROR_EXT("Keyboard key {} {} value is not number.", key, "negative");
      return ESucceed::Failed;
    }
    key_information.neg= neg_it_value.get<unsigned>();
  }

  const auto& gravity_it_value = value.find("gravity");
  if (!gravity_it_value->is_number_unsigned()) {
    PUSH_LOG_ERROR_EXT("Keyboard key {} {} value is not number.", key, "gravity");
    return ESucceed::Failed;
  }
  key_information.neutral_gravity =
      static_cast<float>(gravity_it_value->get<unsigned>());

  const auto& stick_it_value = value.find("stick");
  if (stick_it_value->is_boolean()) {
    key_information.stick_key = stick_it_value.value();
  }

  m_key_inputs.try_emplace(key, std::move(key_information));
  return ESucceed::Succeed;
}

void ProceedGravity(BindingKeyInfo& key_info) {
	const auto dt = opgs16::manager::time::GetDeltaTime();
	auto value = key_info.value;
	key_info.send_signal = false;

	if (value < 0) /** Negative */ {
		value += key_info.neutral_gravity * dt;
		if (-key_info.dead_zone <= value) {
			value = 0.f;
			key_info.key_status = BindingKeyInfo::KeyInputStatus::NEUTRAL;
		}
	}
	else /** Positive */ {
		value -= key_info.neutral_gravity * dt;
		if (value <= key_info.dead_zone) {
			value = 0.f;
			key_info.key_status = BindingKeyInfo::KeyInputStatus::NEUTRAL;
		}
	}

	key_info.value = value;
}

