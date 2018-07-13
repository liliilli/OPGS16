#ifndef MAGICCUP_OBJECT_COMMON_CHOICE_LIST_H
#define MAGICCUP_OBJECT_COMMON_CHOICE_LIST_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <vector>

/// ::opgs16::element::UiObject
#include <Element/ui_object.h>
/// ::opgs16::DColor (float)
#include <Helper/Type/color.h>
#include <Helper/Type/vectori2.h>

namespace opgs16 {
struct DColor;
struct DVectorInt2;
}
namespace opgs16::element::canvas {
class CCanvas;
class CImage;
class CText;
}

namespace magiccup {

enum class EDirection {
  Up,
  Down
};

class ChoiceList final : public opgs16::element::UiObject {
public:
  ChoiceList(const std::string& sprite_name,
             const opgs16::element::canvas::CCanvas* canvas_ref,
             const std::vector<std::string>& choice_list);

  ///
  /// @brief Set font size
  ///
  void SetFontSize(const int32_t size);

  ///
  /// @brief Set font name.
  ///
  void SetFontName(const std::string& font_name);

  ///
  ///
  ///
  void SetItemSize(const int32_t size);

  ///
  ///
  ///
  void SetTextureIndex(const int32_t index);

  ///
  ///
  ///
  void SetCursorSize(const opgs16::DVectorInt2& value);

  ///
  ///
  ///
  void SetSelectedColor(const opgs16::DColor& color_value);

  ///
  ///
  ///
  void SetNormalColor(const opgs16::DColor& color_value);

  ///
  ///
  ///
  void MoveCursor(EDirection direction);

  ///
  ///
  ///
  void SetFunction(int32_t index, std::function<void(void)> callback_function);

  ///
  /// @brief Execute selected command callback. If command does not have callback,
  /// Print warning message and do nothing.
  ///
  void SelectCommand();

  ///
  /// @brief
  ///
  inline int32_t GetCursorIndex() const noexcept {
    return m_cursor_index;
  }

private:
  opgs16::element::canvas::CImage* m_selection_ref = nullptr;

  using TCallback = std::function<void(void)>;
  using TCommandItem = std::pair<opgs16::element::canvas::CText*, TCallback>;
  std::vector<TCommandItem> m_commands = {};

  int32_t m_item_size = 32;
  int32_t m_cursor_index = 0;
  int32_t m_command_list_size = 0;

  opgs16::DColor m_selected_color {};
  opgs16::DColor m_normal_color {};

  void UpdateListItemPosition();
  void UpdateCursorItemPosition(int32_t i);
  void UpdateItemColors();

  void LocalUpdate() override final;
};

} /// ::magiccup namespace

#endif