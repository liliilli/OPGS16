
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/simplelog.h"

#include <Element/Canvas/text.h>

namespace debug::object {

SimpleLog::SimpleLog(int32_t size_value) : m_count{size_value} {
  using opgs16::element::canvas::CText;

  for (int32_t i = 0; i < m_count; ++i) {
    std::string name = s_child_name.data() + std::to_string(i);

    const auto address = this->Instantiate<CText>(name, "");
    address->SetFontName("Bios");
    address->SetFontSize(8);
    address->SetOrigin(Origin::UP_LEFT);
    address->SetAlignment(IAlignable::Alignment::LEFT);

    m_list.emplace_back(name, address);
    m_list.rbegin()->second->SetFontName("Bios");
  }
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
}

void SimpleLog::PushLog(const std::string& log) {
  if (m_index + 1 >= m_count) {
    m_log_list.pop_front();
    m_log_list.emplace_back(log);

    const auto log_item = std::move(m_list.front());
    m_list.pop_front();
    m_list.emplace_back(log_item);

    m_list.rbegin()->second->SetText(*m_log_list.rend());
  }
  else {
    // m_list 을 표현할 때, 일일히 다 갱신하게 하지 말고, CText* 를 뒤로 moving해서
    // 하나만 갱신하게 하면 되잖아..
    // 물론 그러면 리스트 좌표의 재갱신이 필요하긴 함. 근데 이건 불가피함.
    m_log_list.emplace_back(log);
    m_list[m_index].second->SetText(*m_log_list.rend());
    m_index += 1;
  }
}

void SimpleLog::Clear() {
  for (const auto& [_, address] : m_list) {
    address->SetText("");
  }
}

}
