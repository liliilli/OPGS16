#ifndef OPGS16_SYSTEM_MANAGER_PUBLIC_INPUT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_PUBLIC_INPUT_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file Manager/input_manager.h
/// @brief Input manager file
///
/// @author Jongmin Yun
/// @log
/// 2018-03-03 Refactoring.
///

#include <string>			/*! std::string */
#include <unordered_map>	/*! std::unordered_map */

/// ::opgs16::manager::_internal::BindingKeyInfo
#include <Manager\Internal\input_internal.h>
/// Forward declaration
#include <opgs16fwd.h>  

namespace opgs16::manager {

///
/// @class MInputManager
/// @brief This class is singleton and can not be a base of any derived class instance.
/// InputManager has a rule of managing input signal such as keyboard key pressing, releasing and
/// mouse signal or joystick inputs. (Only keyboard input signal checking is implemented. 20180206)
///
/// Any m_object_list want to catch signal of input devices, and performs specific mechanism can use
/// this manager by calling MInputManager::Instance().
/// 
/// @log
/// 2018-03-03 Move BindingKeyInfo to separate file for readability.
///
class MInputManager final {
public:
    using key_map = std::unordered_map<std::string, _internal::BindingKeyInfo>;

public:
	/*!
	 * @brief Static method gets unique instance of Application class.
	 * @return InputManager static instance.
	 */
	static MInputManager& Instance() {
		static MInputManager instance{};
		return instance;
	}

    /*! Initiation routine that must be called. */
    void Initialize(GLFWwindow* window);

	/**
	 * @brief Get Key value which is bound to key container.
	 * This method returns [-1, 1] floating values of key information instance found.
	 * If the key name you find didn't find in container, active error flag. (NOT_FOUND_KEY)
	 * And return always 256.0.
	 *
	 * @param[in] key The key name which key instance has.
	 * @return float The key value which has [-1, 1] range floating value.
	 */
	float GetKeyValue(const std::string& key) const;

	/**
	 * @brief Get whether or not specific key was pressed.
	 * @param[in] key The key name which key instance has.
	 * @return boolean value, if specific key was pressed, return true.
	 */
	bool IsKeyPressed(const std::string& key) const;

	/**
	 * @brief Get whether or not specific key was released.
	 * @param[in] key The key name which key instance has.
	 * @return boolean value, if specific key was released, return true.
	 */
	bool IsKeyReleased(const std::string& key) const;

	/**
	 * @brief Let caller know input_manager has already something like a error.
	 * @return If error is exist, return true else false.
	 */
	bool IsErrorExist();

	/*! Check something has pressed, released. and updates key values going down neutral. */
	void Update();

private:
    GLFWwindow* m_window;       /*! Window handle pointer */
	key_map     m_key_inputs;

	/**
	 * @enum class ErrorFlag
	 * @brief Error flag.
	 */
	enum class ErrorFlag {
		OK,				/** Everything is ok. */
		NOT_FOUND_KEY,	/** This flag will be set up when GetKeyValue() failed. */
	} mutable m_error_flag{ ErrorFlag::OK };

private:
	/**
	 * @brief Let each key value where key status is KeyInputStatus::RELEASED falling down into
	 * dead_zone and change status into KeyInputStatus::NEUTRAL along with neutral_gravity.
	 * This methods gets delta time from Application time data, multiply it with gravity and
	 * fall it down to 0 (neutral value).
	 *
	 * @param[in] key_info Key information to apply.
	 */
	void ProceedGravity(_internal::BindingKeyInfo& key_info);

    /*! Initialize subroutine */
    bool ProceedKeyInit(std::stringstream& stream, _internal::BindingKeyInfo& info);
    bool ProceedKeyInput(std::stringstream& stream, _internal::BindingKeyInfo& info);

    inline bool KeyAlreadyExist(const std::string& name_tag) const {
        return m_key_inputs.find(name_tag) != m_key_inputs.end();
    }

private:
	MInputManager() = default;

public:
	MInputManager(const MInputManager&) = delete;
	MInputManager(const MInputManager&&) = delete;
};

} /*! opgs16::manager */

#define GET_INPUT_MANAGER opgs16::manager::MInputManager::Instance()

#endif /*! OPGS16_SYSTEM_MANAGER_PUBLIC_INPUT_MANAGER_H */