#ifndef OPGS16_MANAGER_INPUT_H
#define OPGS16_MANAGER_INPUT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/input.h
///
/// @brief
/// Manages Input polling, etc.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring.
/// 2018-05-20 Get rid of singleton pattern and rebuild it to namespace.
///

#include <string>

/// Forward declaration
#include <opgs16fwd.h>

///
/// @namespace opgs16::manager::input
///
/// @brief
/// This class is singleton and can not be a base of any derived class instance.
/// InputManager has a rule of managing input signal like keyboard key pressing,
/// releasing and mouse signal or joystick inputs.
///
/// @log
/// 2018-03-03 Move BindingKeyInfo to separate file for readability.
/// 2018-05-20 ::manager::MInputManager type to ::manager::input namespace.
///
/// @todo Implement mouse input system.
///
namespace opgs16::manager::input {

///
/// @enum EKeyPrimaryState
///
///
enum class EKeyPrimaryState : int8_t {
  Released  = 0,
  Pressed   = 1,
  Repeated  = 2
};

///
/// @brief
/// Let load and initiate input setting to be used in application.
/// This function must be called only once out of entire application.
///
void Initiate(GLFWwindow* window_context);

///
/// @brief
/// Get Key value which is bound to key container.
/// This returns [-1, 1] floating values of key information instance found.
/// If the key name you find didn't find in container,
/// output error flag in debug mode, and return always 0.0.
///
/// @param[in] key The key name which key instance has.
/// @return float The key value which has [-1, 1] range floating value.
///
float GetKeyValue(const std::string& key);

///
/// @brief
/// Get whether or not specific key was pressed.
/// @param[in] key The key name which key instance has.
/// @return boolean value, if specific key was pressed, return true.
///
bool IsKeyPressed(const std::string& key);

///
/// @brief
/// Get whether or not specific key was released.
/// @param[in] key The key name which key instance has.
/// @return boolean value, if specific key was released, return true.
///
bool IsKeyReleased(const std::string& key);

///
/// @brief
/// Check something has pressed, released.
/// and updates key values going down neutral.
///
void Update();

} /// ::opgs16::manager::input

#endif /// OPGS16_MANAGER_INPUT_H