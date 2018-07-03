
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneSelect/choice_list.h"

#include <Element/Canvas/image.h>
#include <Element/Canvas/text.h>

#include <Helper/Type/vectori2.h>

namespace debug::object {

ChoiceList::ChoiceList(
    const std::string& sprite_name,
    const opgs16::element::canvas::CCanvas* canvas_ref,
    const std::vector<std::string>& choice_list) {
  using opgs16::element::canvas::CImage;
  using opgs16::element::canvas::CText;

  m_selection_ref = Instantiate<CImage>("Select", sprite_name, canvas_ref);
  const auto length = static_cast<int32_t>(choice_list.size());
  for (int32_t i = 0; i < length; ++i) {
    m_commands.emplace_back(
        Instantiate<CText>("Com" + std::to_string(i), choice_list[i]),
        nullptr
    );
  }

  auto it = std::stable_partition(m_commands.begin(), m_commands.end(),
      [](const TCommandItem& ptr) { return ptr.first != nullptr; });
  m_command_list_size = static_cast<int32_t>(std::distance(m_commands.begin(), it));
  UpdateListItemPosition();
  UpdateCursorItemPosition(m_cursor_index);
}

void ChoiceList::SetFontSize(const int32_t size) {
  for (const auto& item : m_commands) {
    if (item.first == nullptr) break;
    item.first->SetFontSize(size);
  }
}

void ChoiceList::SetFontName(const std::string& font_name) {
  for (const auto& item : m_commands) {
    if (item.first == nullptr) break;
    item.first->SetFontName(font_name);
  }
}

void ChoiceList::SetItemSize(const int32_t size) {
  m_item_size = size;
  UpdateListItemPosition();
  UpdateCursorItemPosition(m_cursor_index);
}

void ChoiceList::SetCursorSize(const opgs16::DVectorInt2& value) {
  m_selection_ref->SetImageSize(
      static_cast<float>(value.x),
      static_cast<float>(value.y));
  UpdateCursorItemPosition(m_cursor_index);
}

void ChoiceList::SetSelectedColor(const opgs16::DColor& color_value) {
  m_selected_color = color_value;
}

void ChoiceList::SetNormalColor(const opgs16::DColor& color_value) {
  m_normal_color = color_value;
}

void ChoiceList::MoveCursor(EDirection direction) {
  switch (direction) {
  case EDirection::Up:
    if (m_cursor_index == 0) break;
    m_cursor_index -= 1;
    UpdateCursorItemPosition(m_cursor_index);
    break;
  case EDirection::Down:
    if (m_cursor_index == m_command_list_size - 1) break;
    m_cursor_index += 1;
    UpdateCursorItemPosition(m_cursor_index);
    break;
  }
}

void ChoiceList::UpdateListItemPosition() {
  float y_pos = static_cast<float>(m_item_size) * (m_command_list_size - 1);
  for (const auto& item : m_commands) {
    if (item.first == nullptr) break;

    item.first->SetWorldPosition({0.f, y_pos, 0.f});
    y_pos -= static_cast<float>(m_item_size);
  }
}

void ChoiceList::UpdateCursorItemPosition(int32_t i) {
  auto scale = m_selection_ref->GetScaleFactor();
  auto y_start = static_cast<float>(m_item_size) * m_command_list_size;
  y_start -= static_cast<float>(m_item_size) * 0.5f;

  const auto y_pos = y_start - static_cast<float>(m_item_size) * i;
  m_selection_ref->SetWorldPosition({ -scale.x - 8.f, y_pos, 0.f});
}

void ChoiceList::LocalUpdate() {
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

} /// ::debug::object namespace