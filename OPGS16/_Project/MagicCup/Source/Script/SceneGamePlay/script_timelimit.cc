
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_timelimit.h"

#include <Component/font2d_renderer.h>
#include <Component/sprite2d_renderer.h>
#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Internal/object_keyword.h"
#include "../../../Include/Object/SceneGamePlay/timer_image.h"
#include "../../../Include/Object/SceneGamePlay/total_management.h"
#include "../../../Include/Script/SceneGamePlay/Management/script_state_machine.h"

namespace {

std::string GetTimerText(int32_t time_value) noexcept {
  const int32_t second = time_value / 1000;
  const int32_t milli3 = (time_value % 1000) / 10;

  std::string string;

  if (second <= 9) {
    string += "0" + std::to_string(second);
  }
  string += ":";
  if (milli3 <= 9) {
    string += "0";
  }
  string += std::to_string(milli3);

  return string;
}

}

namespace magiccup {

void ScriptUiTimelimit::Initiate() {
  using opgs16::component::CSprite2DRenderer;
  using opgs16::element::canvas::CCanvas;
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;
  auto& obj = GetBindObject();

  auto text = obj.CreateGameObject<CText>("Text", "Time");
  text->SetAlignment(IAlignable::Alignment::CENTER);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({0, 20.f, 0});
  text->GetComponent<opgs16::component::CFont2DRenderer>()->SetRenderingLayer(3);
  m_timer_text = text;
  m_timer_text->SetText("Time " + GetTimerText(m_time_value));

  if (auto canvas = GetPresentScene()->GetGameObject(name::canvas)) {
    auto timer = obj.CreateGameObject<TimerImage>(
        TimerImage::s_object_name,
        static_cast<CCanvas*>(canvas));
    timer->SetImageSize(static_cast<float>(m_initial_width), 8.f);
    timer->SetWorldPosition({0.f, 8.f, 0.f});
    m_timer_bar = timer;
  }

  SetComponentActive(false);
}

void ScriptUiTimelimit::ExecuteTimeLimit(int32_t time_value) {
  m_time_set = m_time_value = time_value;
  SetComponentActive(true);
}

void ScriptUiTimelimit::HaltTimeLimit() {
  SetComponentActive(false);
}

int32_t ScriptUiTimelimit::GetTimeValue() const noexcept {
  return m_time_value;
}

void ScriptUiTimelimit::Update(float delta_time) {
  using opgs16::manager::scene::GetPresentScene;

  m_time_value -= delta_time * 1000;
  if (m_time_value <= 0) {
    m_time_value = 0;
    m_timer_text->SetText("Time " + GetTimerText(m_time_value));
    m_timer_bar->SetImageSize(0.f, 8.f);

    HaltTimeLimit();
    GetPresentScene()->GetGameObject(TotalManagement::s_object_name)
        ->GetComponent<ScriptStateMachine>()->
        TransitGameState(EGameState::GameOver);

    return;
  }

  m_timer_text->SetText("Time " + GetTimerText(m_time_value));
  const auto width = static_cast<float>(m_time_value) / m_time_set * m_initial_width;
  m_timer_bar->SetImageSize(width, 8.f);
}


} /// ::magiccup namespace