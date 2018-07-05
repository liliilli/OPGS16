#ifndef DEBUG_PROJECT_OBJECT_COMMON_SUBJECT_H
#define DEBUG_PROJECT_OBJECT_COMMON_SUBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_object.h>

#include <deque>
#include <list>

namespace opgs16::element::canvas {
class CText;
}

namespace debug::object {

class SimpleLog final : public opgs16::element::UiObject {
public:
  SimpleLog(int32_t size_value);

  ///
  /// @brief Set list count to display.
  /// If new value is smaller than previous count, old display will be clapped.
  ///
  void SetCount(int32_t count_value);

  ///
  /// @brief Push log as the lastest.
  ///
  void PushLog(const std::string& log);

  ///
  /// @brief Clear log.
  ///
  void Clear();

private:
  std::list<std::string> m_log_list;
  int32_t m_count = 0;
  int32_t m_index = 0;

  using TListItemType = std::pair<std::string, opgs16::element::canvas::CText*>;
  std::deque<TListItemType> m_list;
  inline static constexpr std::string_view s_child_name = "_";
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_COMMON_SUBJECT_H