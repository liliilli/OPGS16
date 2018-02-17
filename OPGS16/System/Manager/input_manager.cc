#include "input_manager.h"  /*! Header file */

#include <algorithm>
#include <fstream>  /*! std::ifstream */
#include <iostream> /*! std::cout */
#include <sstream>  /*! std::stringstream */

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "time_manager.h"
#define TO_STRING(__ENUM__) #__ENUM__

void InputManager::Initialize(GLFWwindow * window) {
    this->window = window;

    const auto KEY_INIT     = 0u;
    const auto KEY_INPUT    = 1u;
    auto mode{ KEY_INIT };
    auto error{ false };

    std::ifstream in_file_stream(s_input_file, std::ios_base::in);
    //in_file_stream.imbue(std::locale(""));

    if (in_file_stream.good()) {
        std::string line;
        BindingKeyInfo key{};

        std::string error_string{};
        while (std::getline(in_file_stream, line) && (!error)) {
            /*! Neglect comment and line feed */
            if (line.empty()) {
                if (mode == KEY_INPUT) {
                    m_key_inputs.insert(std::make_pair(std::string{ key.name }, std::move(key)));
                    mode = KEY_INIT;
                    key = BindingKeyInfo();
                }
                continue;
            }
            if (line.at(0) == '#') continue;

            std::stringstream stream{ line };
            switch (mode) {
            case KEY_INIT: {
                if (ProceedKeyInit(stream, &key))
                    mode = KEY_INPUT;
                else {
                    error = true;
                    stream.seekg(0);
                    stream >> std::noskipws >> error_string;
                }
            } break;
            case KEY_INPUT: {
                if (!ProceedKeyInput(stream, &key)) {
                    error = true;
                    stream.seekg(0);
                    stream >> std::noskipws >> error_string;
                }
            } break;
            }
        }

        if (error) {    /*! If an error occured , display */
            std::cerr << "ERROR::OCCURED::" << error_string << "\n";
        }
    }
}

bool InputManager::ProceedKeyInit(std::stringstream& stream,
                                  InputManager::BindingKeyInfo* const info) {
    std::string token; stream >> token;
    if (!(token == "KB" || token == "MS" || token == "JS"))
        /*! Break and display error message */
        assert(false);

    stream >> token;
    /*! Check token name already exist in m_key_inputs */
    if (std::any_of(m_key_inputs.cbegin(), m_key_inputs.cend(),
                    [new_name = token](auto& key) { return key.second.name == new_name; }
    )) { return false; }

    info->name = token;
    return true;
}

bool InputManager::ProceedKeyInput(std::stringstream& stream, BindingKeyInfo* const info) {
    std::string token; stream >> token;
    if (token == "+" || token == "-") {
        int* bind_pos{ nullptr };
        if (token == "+") bind_pos = &info->pos;
        else bind_pos = &info->neg;

        std::string key_string; stream >> key_string;

        if (key_string == TO_STRING(GLFW_KEY_ESCAPE))
            *bind_pos = GLFW_KEY_ESCAPE;
        else if (key_string == TO_STRING(GLFW_KEY_F1))
            *bind_pos = GLFW_KEY_F1;
        else if (key_string == TO_STRING(GLFW_KEY_F2))
            *bind_pos = GLFW_KEY_F2;
        else if (key_string == TO_STRING(GLFW_KEY_F3))
            *bind_pos = GLFW_KEY_F3;
        else if (key_string == TO_STRING(GLFW_KEY_F9))
            *bind_pos = GLFW_KEY_F9;
        else if (key_string == TO_STRING(GLFW_KEY_F10))
            *bind_pos = GLFW_KEY_F10;
        else if (key_string == TO_STRING(GLFW_KEY_RIGHT))
            *bind_pos = GLFW_KEY_RIGHT;
        else if (key_string == TO_STRING(GLFW_KEY_LEFT))
            *bind_pos = GLFW_KEY_LEFT;
        else if (key_string == TO_STRING(GLFW_KEY_UP))
            *bind_pos = GLFW_KEY_UP;
        else if (key_string == TO_STRING(GLFW_KEY_DOWN))
            *bind_pos = GLFW_KEY_DOWN;
    }
    else if (token == "g") {
        stream >> token;
        info->neutral_gravity = static_cast<float>(std::stoi(token));
    }
    else return false;

    return true;
}


float InputManager::GetKeyValue(const std::string & key) {
	if (m_key_inputs.find(key) != m_key_inputs.end()) {
		return m_key_inputs.at(key).value;
	}
	else { // Set error flag.
		m_error_flag = ErrorFlag::NOT_FOUND_KEY;
		return 0.f;
	}
}

bool InputManager::IsKeyPressed(const std::string & key) {
	auto it = m_key_inputs.find(key);
	if (it != m_key_inputs.end()) {
		auto& key = (*it).second;
		switch (key.key_status) {
		case BindingKeyInfo::KeyInputStatus::NEG_PRESSED:
		case BindingKeyInfo::KeyInputStatus::POS_PRESSED:
			if (!key.stick_key) return true;
			// Key has stick key property.
			if (!key.send_signal) {
				key.send_signal = true;
				return true;
			}
			else return false;
		default: return false;
		}
	}
	else {
		m_error_flag = ErrorFlag::NOT_FOUND_KEY;
		return false;
	}
}

bool InputManager::IsKeyReleased(const std::string & key) {
	if (m_key_inputs.find(key) != m_key_inputs.end()) {
		switch (m_key_inputs.at(key).key_status) {
		case BindingKeyInfo::KeyInputStatus::NEUTRAL:
		case BindingKeyInfo::KeyInputStatus::RELEASED:
			return true;
		default: return false;
		}
	}
	else {
		m_error_flag = ErrorFlag::NOT_FOUND_KEY;
		return false;
	}
}

bool InputManager::IsErrorExist() {
	return m_error_flag != ErrorFlag::OK;
}

void InputManager::Update() {
	for (auto& key_info : m_key_inputs) {
		auto& key = key_info.second;
		// PRESSED, NEUTRAL checks key pressed event.
		// If key released in state PRESSED, change it to RELEASED.
		using Status = BindingKeyInfo::KeyInputStatus;

		switch (key.key_status) {
		case Status::NEUTRAL:
			if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.neg) == GLFW_PRESS) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.pos) == GLFW_PRESS) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			break;
		case Status::NEG_PRESSED:
			if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.pos) == GLFW_PRESS) {
				key.value = 1.0f;
				key.key_status = Status::POS_PRESSED;
			}
			else if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.neg) == GLFW_RELEASE) {
				key.key_status = Status::RELEASED;
				ProceedGravity(key);
			}
			break;
		case Status::POS_PRESSED:
			if (key.neg != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.neg) == GLFW_PRESS) {
				key.value = -1.0f;
				key.key_status = Status::NEG_PRESSED;
			}
			else if (key.pos != GLFW_KEY_UNKNOWN && glfwGetKey(window, key.pos) == GLFW_RELEASE) {
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

void InputManager::ProceedGravity(BindingKeyInfo & key_info) {
	const auto dt = TimeManager::GetInstance().GetDeltaTime();
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
