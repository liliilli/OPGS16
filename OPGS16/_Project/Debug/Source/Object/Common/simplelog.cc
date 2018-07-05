
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/simplelog.h"

#include <Element/Canvas/text.h>
#include <Phitos/Dbg/assert.h>

namespace debug::object {

SimpleLog::SimpleLog(int32_t size_value) : m_count{size_value} {
  using opgs16::element::canvas::CText;

  for (int32_t i = 0; i < m_count; ++i) {
    std::string name = s_child_name.data() + std::to_string(i);

    const auto address = this->Instantiate<CText>(name, "");
    address->SetFontName("opSystem");
    address->SetFontSize(8);
    address->SetOrigin(Origin::UP_LEFT);
    address->SetAlignment(IAlignable::Alignment::LEFT);

    m_list.emplace_back(name, address);
  }

  UpdateListItemPosition();
}

void SimpleLog::SetCount(int32_t count_value) {
  Clear();

  if (count_value < m_count) {
    for (int32_t i = m_count - 1; i >= count_value; --i) {
      this->DestroyChild(m_list[i].first);
      m_list.resize(count_value);
    }
  }

  m_count = count_value;
  m_index = 0;

  UpdateListItemPosition();
}

void SimpleLog::PushLog(const std::string& log) {
  if (m_index >= m_count) {
    m_log_list.pop_front();
    m_log_list.emplace_back(log);

    const auto log_item = std::move(m_list.front());
    m_list.pop_front();
    m_list.emplace_back(log_item);
    m_list.rbegin()->second->SetText(*m_log_list.rbegin());

    UpdateListItemPosition();
  }
  else {
    m_log_list.emplace_back(log);
    m_list[m_index].second->SetText(*m_log_list.rbegin());

    m_index += 1;
  }
}

void SimpleLog::Clear() {
  for (const auto& [_, address] : m_list) {
    address->SetText("");
  }
}

void SimpleLog::SetHeight(const int32_t size) {
  PHITOS_ASSERT(size > 0, "Log size must be higher than 0.");
  UpdateListItemPosition();
}

void SimpleLog::UpdateListItemPosition() {
  int32_t count = 1;
  for (auto& [name, element] : m_list) {
    element->SetWorldPosition({0.f, static_cast<float>(-m_height * count), 0.f});
    ++count;
  }
}

void SimpleLog::LocalUpdate() {
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

}
