/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Manager/Private/input_manager.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-03 Refactoring.
 */

#include <Manager\input_manager.h>  /// Header file

#include <fstream>      /*! std::ifstream */
#include <iostream>     /*! std::cout */
#include <sstream>      /*! std::stringstream */
#include <string_view>  /*! std::string_view */

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <Manager\time_manager.h>   /// Header file

namespace opgs16 {

namespace {
using namespace std::string_view_literals;
constexpr std::string_view input_path{ "_Setting/input.meta"sv };

/*! Status when input setting file loading */
enum class LoadStatus {
    KEY_INIT,
    KEY_INPUT
};

} /*! unnamed namespace */

namespace manager {
using _internal::BindingKeyInfo;

void MInputManager::Initialize(GLFWwindow* window) {
    m_window = window;

    std::ifstream file_stream(opgs16::input_path.data(), std::ios_base::in);
    file_stream.imbue(std::locale(""));

    if (file_stream.good()) {
        BindingKeyInfo key{};
        std::string error_string{};

        auto status{ LoadStatus::KEY_INIT };
        auto error{ false };

        std::string line_token;
        while (std::getline(file_stream, line_token) && (!error)) {
            /*! Neglect comment and line feed */
            if (line_token.empty()) {
                if (status == LoadStatus::KEY_INPUT) {
                    m_key_inputs.emplace(std::string{ key.name }, std::move(key));
                    status = LoadStatus::KEY_INIT;
                    key = BindingKeyInfo();
                }
                continue;
            } else if (line_token.at(0) == '#') continue;

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

bool MInputManager::ProceedKeyInit(std::stringstream& stream, BindingKeyInfo& info) {
    /*! Check input style (KB, MS, JS) */
    {
        std::string input_style; stream >> input_style;

        if (!(input_style == "KB" || input_style == "MS" || input_style == "JS"))
            assert(false);
        else info.key_type = _internal::GetKeyType(input_style);
    }

    /*! Check token name already exist in m_key_inputs */
    std::string name_tag; stream >> name_tag;
    if (KeyAlreadyExist(name_tag)) return false;
    else {
        info.name = name_tag;
        return true;
    }
}

bool MInputManager::ProceedKeyInput(std::stringstream& stream, BindingKeyInfo& info) {
    std::string token; stream >> token;
    if (token == "+" || token == "-") {
        int* bind_pos{ nullptr };
        if (token == "+") bind_pos = &info.pos;
        else bind_pos = &info.neg;

        int key_token; stream >> key_token;
        *bind_pos = key_token;
        return true;
    }
    else if (token == "g") {
        float gravity; stream >> gravity;
        info.neutral_gravity = gravity;
        return true;
    }
    else return false;
}

float MInputManager::GetKeyValue(const std::string& key) const {
	if (m_key_inputs.find(key) != m_key_inputs.end()) {
		return m_key_inputs.at(key).value;
	}
	else { // Set error flag.
		m_error_flag = ErrorFlag::NOT_FOUND_KEY;
		return 0.f;
	}
}

bool MInputManager::IsKeyPressed(const std::string & key) const {
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

bool MInputManager::IsKeyReleased(const std::string & key) const {
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

bool MInputManager::IsErrorExist() {
	return m_error_flag != ErrorFlag::OK;
}

void MInputManager::Update() {
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

void MInputManager::ProceedGravity(BindingKeyInfo & key_info) {
	const auto dt = MTimeManager::Instance().GetDeltaTime();
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

} /*! opgs16::manager */
} /*! opgs16 */

