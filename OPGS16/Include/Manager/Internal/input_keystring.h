#ifndef OPGS16_MANAGER_INTERNAL_INPUT_KEYSTRING_H
#define OPGS16_MANAGER_INTERNAL_INPUT_KEYSTRING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/input_keystring.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-02 Create file.
///

#include <optional>

namespace opgs16::manager::input {

using TInputKeyPair = std::pair<const char*, int32_t>;

constexpr std::array<TInputKeyPair, 77> s_key_string = {
  TInputKeyPair\
  {"KB_RIGHT", 262},
  {"KB_LEFT", 263},
  {"KB_DOWN", 264},
  {"KB_UP", 265},

  {"KB_A", 65},
  {"KB_B", 66},
  {"KB_C", 67},
  {"KB_D", 68},
  {"KB_E", 69},
  {"KB_F", 70},
  {"KB_G", 71},
  {"KB_H", 72},
  {"KB_I", 73},
  {"KB_J", 74},
  {"KB_K", 75},
  {"KB_L", 76},
  {"KB_M", 77},
  {"KB_N", 78},
  {"KB_O", 79},
  {"KB_P", 80},
  {"KB_Q", 81},
  {"KB_R", 82},
  {"KB_S", 83},
  {"KB_T", 84},
  {"KB_U", 85},
  {"KB_V", 86},
  {"KB_W", 87},
  {"KB_X", 88},
  {"KB_Y", 89},
  {"KB_Z", 90},

  {"KB_0", 48},
  {"KB_1", 49},
  {"KB_2", 50},
  {"KB_3", 51},
  {"KB_4", 52},
  {"KB_5", 53},
  {"KB_6", 54},
  {"KB_7", 55},
  {"KB_8", 56},
  {"KB_9", 57},

  {"KB_ESCAPE", 256},
  {"KB_ENTER", 257},

  {"KB_LSHIFT", 340},
  {"KB_LCTRL", 341},
  {"KB_LALT", 342},

  {"KB_INSERT", 260},
  {"KB_DELETE", 261},
  {"KB_PAGEUP", 266},
  {"KB_PAGEDOWN", 267},
  {"KB_HOME", 268},
  {"KB_END", 269},

  {"KB_MINUS", 45},
  {"KB_EQUAL", 61},
  {"KB_LBRACKET", 91},
  {"KB_RBRACKET", 93},

  {"KB_SEMICOLON", 59},
  {"KB_APOSTROPHE", 39},
  {"KB_COMMA", 44},
  {"KB_PERIOD", 46},
  {"KB_SLASH", 47},

  {"KB_F1", 290},
  {"KB_F2", 291},
  {"KB_F3", 292},
  {"KB_F4", 293},
  {"KB_F5", 294},
  {"KB_F6", 295},
  {"KB_F7", 296},
  {"KB_F8", 297},
  {"KB_F9", 298},
  {"KB_F10", 299},
  {"KB_F11", 300},
  {"KB_F12", 301},

  {"KB_CAPSLOCK", 280},
  {"KB_TAB", 258},
  {"KB_BACKSPACE", 259},
  {"KB_BACLSLASH", 92},
  {"KB_GBACCENT", 96}
};

inline std::optional<int32_t> GetKeyUidValue(const std::string& key_string) {
  for (auto& [c_str, uid] : s_key_string) {
    if (key_string == c_str)
      return uid;
  }

  return std::nullopt;
}

} /// ::opgs16::manager::input namespace

#endif /// OPGS16_MANAGER_INTERNAL_INPUT_KEYSTRING_H