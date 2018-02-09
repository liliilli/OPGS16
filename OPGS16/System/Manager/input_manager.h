#ifndef OPGS16_SYSTEM_MANAGER_INPUT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_INPUT_MANAGER_H

/**
 * @file System\Manager\input_manager.h
 * @brief
 *
 * @author Jongmin Yun
 * @date 2018 - 02 - 06
 */

#include <string>			/** std::string */
#include <unordered_map>	/** std::unordered_map */
#include "..\..\Headers\Fwd\objectfwd.h" /*! GFLWwindow */

/*!
 * @class InputManager
 * @brief This class is singleton and can not be a base of any derived class instance.
 * InputManager has a rule of managing input signal such as keyboard key pressing, releasing and
 * mouse signal or joystick inputs. (Only keyboard input signal checking is implemented. 20180206)
 *
 * Any objects want to catch signal of input devices, and performs specific mechanism can use
 * this manager by calling InputManager::GetInstance().
 *
 * @date 2018 - 02 - 06
 */
class InputManager final {
public:
	/**
	 * @brief Static method gets unique instance of Application class.
	 * @return InputManager static instance.
	 */
	static InputManager& GetInstance(GLFWwindow* window) {
		static InputManager instance{ window };
		return instance;
	}

	/**
	 * @brief Get Key value which is bound to key container.
	 * This method returns [-1, 1] floating values of key information instance found.
	 * If the key name you find didn't find in container, active error flag. (NOT_FOUND_KEY)
	 * And return always 256.0.
	 *
	 * @param[in] key The key name which key instance has.
	 * @return float The key value which has [-1, 1] range floating value.
	 */
	float GetKeyValue(const std::string& key);

	/**
	 * @brief Get whether or not specific key was pressed.
	 * @param[in] key The key name which key instance has.
	 * @return boolean value, if specific key was pressed, return true.
	 */
	bool IsKeyPressed(const std::string& key);

	/**
	 * @brief Get whether or not specific key was released.
	 * @param[in] key The key name which key instance has.
	 * @return boolean value, if specific key was released, return true.
	 */
	bool IsKeyReleased(const std::string& key);

	/**
	 * @brief Let caller know input_manager has already something like a error.
	 * @return If error is exist, return true else false.
	 */
	bool IsErrorExist();

	/**
	 * @brief Check something has pressed, released. and updates key values going down neutral.
	 */
	[[noreturn]] void Update();

private:
	/**
	 * @struct BindingKeyInfo
	 * @brief Stores key info to use in this game, and overall application.
	 * You can bind arbitary (Supported on GLFW) key into key container, and get values from
	 * them to call input_event handler.
	 *
	 * If the user presses negative key, key information instance bound to key container
	 * set value to -1.0f. Inversely as positive key is.
	 * And the user releases negative or positive key, key information instance let values
	 * go to neurtal zone. (within range of dead_zone)
	 */
	struct BindingKeyInfo {
		using negative_button = int;
		using positive_button = int;

		/**
		 * @enum class KeyInputStatus
		 * @brief Indicates key status of keys.
		 */
		enum class KeyInputStatus : int {
			POS_PRESSED = 1,	// If positive key is pressed, key_status will sustain PRESSED.
			NEG_PRESSED = 2,	// If negative key is pressed, key_status will sustain PRESSED.
			RELEASED = 3,	// Changes to RELEASED when released. constraints of Update.
			NEUTRAL = 4,	// If released, and within range of dead_zone, NEURTAL.
		} key_status{ KeyInputStatus::NEUTRAL };

		bool			stick_key{ true };			// If true, pressed key sends signal only once.
		bool			send_signal{ false };

		std::string		name{};	// container key name must be same as structure's name. (Camel)
		negative_button neg{ -1 };	// Negative button key information.
		negative_button pos{ -1 };	// Positive button key information.
		float			neutral_gravity{ 1000.f };	// Gravity to drag value down to neutral zone.
		float			dead_zone{ 0.001f };		// The range of neutral zone.
		float			value{ 0.f };				// Value moves along with neg/pos/gravity.
	};

	std::unordered_map<std::string, BindingKeyInfo> m_key_inputs;

	/**
	 * @enum class ErrorFlag
	 * @brief Error flag.
	 */
	enum class ErrorFlag {
		OK,				/** Everything is ok. */
		NOT_FOUND_KEY,	/** This flag will be set up when GetKeyValue() failed. */
	} m_error_flag{ ErrorFlag::OK };

    GLFWwindow* const window; /** Window handle pointer */

private:
	/**
	 * @brief Let each key value where key status is KeyInputStatus::RELEASED falling down into
	 * dead_zone and change status into KeyInputStatus::NEUTRAL along with neutral_gravity.
	 * This methods gets delta time from Application time data, multiply it with gravity and
	 * fall it down to 0 (neutral value).
	 *
	 * @param[in] key_info Key information to apply.
	 */
	[[noreturn]] void ProceedGravity(BindingKeyInfo& key_info);

private:
	InputManager(GLFWwindow* window);
	InputManager(const InputManager&) = delete;
	InputManager(const InputManager&&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(const InputManager&&) = delete;
};

#endif /** OPGS16_SYSTEM_MANAGER_INPUT_MANAGER_H */