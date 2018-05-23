
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/input.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring.
/// 2018-05-20 Get rid of singleton pattern and rebuild it to namespace.
///
/// @todo Replace own input data style to json.
///

#include <Manager\input_manager.h>  /// Header file

#include <fstream> 
#include <sstream>
#include <string_view>  
#include <unordered_map>	

#include <GL\glew.h>
#include <GLFW\glfw3.h>

/// ::opgs16;:core::application
#include <Core\application.h>
/// ::opgs16::SGlobalSetting
#include <Core\core_setting.h>

/// Import logger
#include <Headers\import_logger.h>
/// Expanded assertion
#include <Helper\assert.h>
/// Header file
#include <Manager\time_manager.h>
/// ::opgs16::debug error messages.
#include <Manager\Internal\error_message.h>
/// ::opgs16::manager::_internal flags
#include <Manager\Internal\flag.h>
/// ::opgs16::manager::_internal::BindingKeyInfo
#include <Manager\Internal\input_internal.h>

///
/// @enum EKeyExist
/// @brief
///
enum class EKeyExist : bool {
  NotExist = false,
  Exist = true
};

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward Declaration
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// @brief
/// 
void ReadInputFile(const char* file_path);

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
/// Initiate subroutine.
///
/// @param[in] stream
///
/// @param[in] info
///
bool ProceedKeyInit(std::stringstream& stream, 
                    opgs16::manager::_internal::BindingKeyInfo& info);

///
/// @brief
/// Initiate subroutine.
///
/// @param[in] stream
///
/// @param[in] info
///
bool ProceedKeyInput(std::stringstream& stream, 
                     opgs16::manager::_internal::BindingKeyInfo& info);

///
/// @brief
/// 
/// @param[in] key 
/// 
EKeyExist IsKeyExist(const std::string& key);

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

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
using namespace std::string_view_literals;
constexpr std::string_view input_path{ "_Setting/input.meta"sv };

using key_map = std::unordered_map<std::string, BindingKeyInfo>;

// Status when input setting file loading
enum class LoadStatus {
  KEY_INIT,
  KEY_INPUT
};

// Window handle pointer
GLFWwindow* m_window;       
GLFWcursor* m_cursor = nullptr;

key_map m_key_inputs;

} /// unnamed namespace

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
void __InputKeyCallback(GLFWwindow* window, 
                        int key, int scancode, int action, int mod) {
  PUSH_LOG_INFO_EXT("Key input : {0}, {1}", key, action);
#if defined(false)
  // @bug DEBUG_EXT does not output log message on console even in dbg mode.
  PUSH_LOG_DEBUG_EXT("Key input : {0}, {1}", key, action);
#endif
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
void __MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
  const auto& setting = opgs16::entry::Setting();
  const auto scale_value = setting.ScaleValueIntegerOf();

  const auto regulated_xpos = xpos / scale_value;
  const auto regulated_ypos = ypos / scale_value;
  
  PUSH_LOG_INFO_EXT(
      "Mouse position update : O {0:2}, {1:2}, R {2:2}, {3:2}", 
      xpos, ypos, regulated_xpos, regulated_ypos);
}

namespace opgs16::manager::input {

void Initiate(GLFWwindow* window_context) {
  NEU_ASSERT(m_initiated == EInitiated::NotInitiated,
      debug::err_input_duplicated_init);
  m_initiated = EInitiated::Initiated;

  m_window = window_context;
  glfwSetKeyCallback(m_window, __InputKeyCallback);
  glfwSetCursorPosCallback(m_window, __MousePositionCallback);

  unsigned char m_data[16 * 16 * 4];
  memset(m_data, 0xFF, sizeof(m_data));
  GLFWimage image;
  image.height = 16;
  image.width = 16;
  image.pixels = m_data;
  m_cursor = glfwCreateCursor(&image, 0, 0);

  glfwSetCursor(m_window, m_cursor);

  ReadInputFile(input_path.data());
}

float GetKeyValue(const std::string& key) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_input_not_initiated);

  if (IsKeyExist(key) == EKeyExist::NotExist) {
#if defined(_DEBUG)
    PUSH_LOG_ERRO("fuck");
#endif
		return 0.f;
	}

  return m_key_inputs[key].value;
}

bool IsKeyPressed(const std::string& key) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_input_not_initiated);

  if (m_key_inputs.find(key) == m_key_inputs.end()) {
#if defined(_DEBUG)
    PUSH_LOG_ERRO("fuck");
#endif
		return false;
  }

  BindingKeyInfo& key_info = m_key_inputs[key];

  switch (key_info.key_status) {
  case BindingKeyInfo::KeyInputStatus::NEG_PRESSED:
  case BindingKeyInfo::KeyInputStatus::POS_PRESSED:
    if (!key_info.stick_key) 
      return true;

    // Key has stick key property.
    if (!key_info.send_signal) {
      key_info.send_signal = true;
      return true;
    }
    else 
      return false;
  default: 
    return false;
  }
}

bool IsKeyReleased(const std::string& key) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_input_not_initiated);

  if (IsKeyExist(key) == EKeyExist::NotExist) {

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
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_input_not_initiated);

	for (auto& key_info : m_key_inputs) {
		auto& key = key_info.second;
		// PRESSED, NEUTRAL checks key pressed event.
		// If key released in state PRESSED, change it to RELEASED.
		using Status = BindingKeyInfo::KeyInputStatus;

		switch (key.key_status) {
		case Status::NEUTRAL:
			if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.neg) == GLFW_PRESS) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.pos) == GLFW_PRESS) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			break;
		case Status::NEG_PRESSED:
			if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.pos) == GLFW_PRESS) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			else if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.neg) == GLFW_RELEASE) {
				key.key_status = Status::RELEASED;
				ProceedGravity(key);
			}
			break;
		case Status::POS_PRESSED:
			if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.neg) == GLFW_PRESS) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(m_window, key.pos) == GLFW_RELEASE) {
				key.key_status = Status::RELEASED;
				ProceedGravity(key);
			}
			break;
		case Status::RELEASED:
			ProceedGravity(key);
			break;
		}
	}
}

} /// opgs16::manager::input

void ReadInputFile(const char* file_path) {
  std::ifstream file_stream(file_path, std::ios_base::in);
  file_stream.imbue(std::locale(""));

  if (file_stream.good()) {
    BindingKeyInfo key{};
    std::string error_string{};

    auto status{ LoadStatus::KEY_INIT };
    auto error{ false };

    std::string line_token;
    while (std::getline(file_stream, line_token) && (!error)) {
      // Pass comment and line feed
      if (line_token.empty()) {
        if (status == LoadStatus::KEY_INPUT) {
          m_key_inputs.emplace(std::string{ key.name }, std::move(key));
          status = LoadStatus::KEY_INIT;
          key = BindingKeyInfo();
        }
        continue;
      }
      else if (line_token.at(0) == '#') continue;

      switch (std::stringstream line_stream{ line_token }; status) {
      case LoadStatus::KEY_INIT: {
        if (!ProceedKeyInit(line_stream, key)) {
          error = true;
          line_stream.seekg(0);
          line_stream >> std::noskipws >> error_string;
        }
        else {
          status = LoadStatus::KEY_INPUT;
        }
      } break;
      case LoadStatus::KEY_INPUT: {
        if (!ProceedKeyInput(line_stream, key)) {
          error = true;
          line_stream.seekg(0);
          line_stream >> std::noskipws >> error_string;
        }
      } break;
      }
    }

    if (error) {    /*! If an error occured , display */
      std::cerr << "ERROR::OCCURED::" << error_string << "\n";
    }
  }
}

bool ProceedKeyInit(std::stringstream& stream, BindingKeyInfo& info) {
  // Check input style (KB, MS, JS)
  std::string input_style; 
  stream >> input_style;

  if (!(input_style == "KB" || input_style == "MS" || input_style == "JS"))
    assert(false);
  else 
    info.key_type = opgs16::manager::_internal::GetKeyType(input_style);

  // Check token name already exist in m_key_inputs
  std::string key; 
  stream >> key;

  if (IsKeyExist(key) == EKeyExist::Exist)
    return false;
  else {
    info.name = key;
    return true;
  }
}

bool ProceedKeyInput(std::stringstream& stream, BindingKeyInfo& info) {
  std::string token; 
  stream >> token;

  if (token == "+" || token == "-") {
    int* bind_pos;
    if (token == "+") 
      bind_pos = &info.pos;
    else 
      bind_pos = &info.neg;

    int key_token; 
    stream >> key_token;
    *bind_pos = key_token;
    return true;
  }
  else if (token == "g") {
    float gravity; 
    stream >> gravity;
    info.neutral_gravity = gravity;
    return true;
  }

  return false;
}


void ProceedGravity(BindingKeyInfo & key_info) {
	const auto dt = opgs16::manager::MTimeManager::Instance().GetDeltaTime();
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

