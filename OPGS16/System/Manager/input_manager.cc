#include "input_manager.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "time_manager.h"

InputManager::InputManager(GLFWwindow* window) : window{ window } {

	BindingKeyInfo&& g_GlobalCancel{}; {
		g_GlobalCancel.name = "GlobalCancel";
		g_GlobalCancel.pos	= GLFW_KEY_ESCAPE;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalCancel.name },
										   std::move(g_GlobalCancel)));
	}
	BindingKeyInfo&& g_GlobalF1{}; {
		g_GlobalF1.name = "GlobalF1";
		g_GlobalF1.pos = GLFW_KEY_F1;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalF1.name },
										   std::move(g_GlobalF1)));
	}
	BindingKeyInfo&& g_GlobalF2{}; {
		g_GlobalF2.name = "GlobalF2";
		g_GlobalF2.pos = GLFW_KEY_F2;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalF2.name },
										   std::move(g_GlobalF2)));
	}
	BindingKeyInfo&& g_GlobalF3{}; {
		g_GlobalF3.name = "GlobalF3";
		g_GlobalF3.pos = GLFW_KEY_F3;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalF3.name },
										   std::move(g_GlobalF3)));
	}
	BindingKeyInfo&& g_GlobalF9{}; {
		g_GlobalF9.name = "GlobalF9";
		g_GlobalF9.pos = GLFW_KEY_F9;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalF9.name },
										   std::move(g_GlobalF9)));
	}
	BindingKeyInfo&& g_GlobalF10{}; {
		g_GlobalF10.name = "GlobalF10";
		g_GlobalF10.pos = GLFW_KEY_F10;
		m_key_inputs.insert(std::make_pair(std::string{ g_GlobalF10.name },
										   std::move(g_GlobalF10)));
	}
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

