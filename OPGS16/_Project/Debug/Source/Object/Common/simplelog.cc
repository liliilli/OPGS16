
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
    // m_list �� ǥ���� ��, ������ �� �����ϰ� ���� ����, CText* �� �ڷ� moving�ؼ�
    // �ϳ��� �����ϰ� �ϸ� ���ݾ�..
    // ���� �׷��� ����Ʈ ��ǥ�� �簻���� �ʿ��ϱ� ��. �ٵ� �̰� �Ұ�����.
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
