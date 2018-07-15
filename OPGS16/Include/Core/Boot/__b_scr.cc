
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/Boot/__b_scr.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-07 Activate file and add comments.
/// 2018-04-14 Move definitions into ::opgs16::builtin::sample namespace.
///

/// Header file
#include "__b_scr.h"

#include <chrono>
#include <random>
#include <sstream>

#include <glm/glm.hpp>

#include <Component/sprite2d_renderer.h>
#include <Core/core_setting.h>
#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Element/Canvas/image.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>
#include <Shader/shader_wrapper.h>
#include <Headers/import_logger.h>
#include <Helper/Math/random.h>

#include <../manifest.h>

namespace {

constexpr const char*   shader_sliced{ "__b_sliced" };
constexpr const int32_t k_sliced_number{ 32 };
constexpr const float   k_sliced_y_initial_scale{ 128.f };
constexpr const float   k_sliced_y_final_scale{ 9.5f };

int y_positions[k_sliced_number << 1]{ 0, };
int y_initial[k_sliced_number << 1]{ 0, };

} /// unnamed namespace

namespace opgs16::builtin::sample {

void __B_SCR::Start() {
  OP16_TIMER_SET(m_timer_2, 1'000, false, this, &__B_SCR::SetLogoImage);
}

void __B_SCR::SetLogoImage() {
  auto& bind_obj = GetBindObject();

  logo = bind_obj.CreateGameObject<element::canvas::CImage>(
      "Logo", "opSystem",
      static_cast<element::canvas::CCanvas*>(&bind_obj));

  auto* renderer = logo->GetComponent<component::CSprite2DRenderer>();
  renderer->SetTextureFragmentIndex(4);
  logo->SetImageSize(178.f, 19.f);

  using random::RandomIntegerRange;
  if (RandomIntegerRange(0, 100) < 50) {
    // Normal
    logo->SetWorldPosition({ 0.f, 160.f, 0.f });
    OP16_TIMER_SET(m_timer, 32, true, this, &__B_SCR::MoveLogo1);
  }
  else {
    // Sliced
    for (int32_t i = 0; i < (k_sliced_number << 1); ++i) {
      y_positions[i]  = RandomIntegerRange(0, 400) + 312;
      y_initial[i]    = y_positions[i];
    }

    renderer->SetShader(shader_sliced);
    renderer->SetInstanceCount(k_sliced_number << 1);
    auto& wrapper = renderer->GetWrapper();
    wrapper.SetUniformValueInt("uNumber", k_sliced_number << 1);
    wrapper.SetUniformValueIntPtr("uPos", y_positions, k_sliced_number << 1);
    wrapper.SetUniformValue<float>("uYScale", k_sliced_y_initial_scale);
    OP16_TIMER_SET(m_timer, 16, true, this, &__B_SCR::MoveLogoSliced);
  }
}

void __B_SCR::Destroy() {
  auto& bind_obj = GetBindObject();

  OP16_TIMER_STOP(m_timer);
  OP16_TIMER_STOP(m_timer_2);

  bind_obj.DestroyGameObject("Logo");
  bind_obj.DestroyGameObject("Statement");
}

void __B_SCR::MoveLogo1() {
  static float elapsed{ 0.f };
  auto logo = GetBindObject().GetGameObject("Logo");
  if (!logo) return;

  auto pos = logo->GetWorldPosition();
  constexpr float total = 2.0f;
  constexpr float mult = 160.f / total;
  elapsed += 0.032f;
  pos.y = powf(elapsed - total, 2) * mult;
  if (abs(pos.y) < 0.1f) {
    PUSH_LOG_INFO("Logo soft-randing");
    pos.y = 0;
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer_2, 500, false, this, &__B_SCR::CreateTextObject);
  }

  logo->SetWorldPosition(pos);
}

void __B_SCR::MoveLogoSliced() {
  using opgs16::component::CSprite2DRenderer;

  static float elapsed{ 0.f };
  constexpr float time{ 0.75f };
  constexpr float time_pow{ 0.5625f };
  constexpr float offset_scale{ k_sliced_y_initial_scale / time_pow };

  elapsed += 0.016f;
  const auto sigma = powf(time - elapsed, 2);
  for (int i = 0; i < (k_sliced_number << 1); ++i)
    y_positions[i] = static_cast<int>(sigma * (y_initial[i] / time_pow));

  auto* renderer = logo->GetComponent<CSprite2DRenderer>();
  auto& wrapper = renderer->GetWrapper();

  wrapper.SetUniformValue<float>(
      "uYScale", sigma / offset_scale + k_sliced_y_final_scale);

  if (elapsed >= time) {
    PUSH_LOG_INFO("Logo soft-randing");
    renderer->SetShader("gQuad");
    renderer->SetInstanceCount(1);
    OP16_TIMER_STOP(m_timer);
    OP16_TIMER_SET(m_timer_2, 500, false, this, &__B_SCR::CreateTextObject);
  }
}

void __B_SCR::CreateTextObject() {
  std::stringstream string_stream;
  string_stream << 'v' << _OPGS16_VERSION_MAJOR << '.' << _OPGS16_VERSION_MINOR << '.' << _OPGS16_VERSION_FIXED;

  std::string text_string; string_stream >> text_string;
  text_string.append("\n\n"
    "VIDEO RAM:1024KiBytes\n"
    "MAIN RAN:2048KiByes\n\n");

  auto text = std::make_unique<element::canvas::CText>(text_string); {
    text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
    text->SetWorldPosition({ 0, -48, 0 });
    text->SetAlignment(IAlignable::Alignment::CENTER);
    text->SetFontName("opSystem");
    text->SetFontSize(8u);
    text->SetColor(DColor{ 1, 1, 1 });
  }
  GetBindObject().CreateGameObject<element::canvas::CText>("Statement", text);
  OP16_TIMER_SET(m_timer, 2'000, false, this, &__B_SCR::OnTriggerNextScene);
}

void __B_SCR::OnTriggerNextScene() {
    PUSH_LOG_INFO("Booting scene to sample game scene...");
#if !defined(_CUSTOM_PROJECT)
    //M_REPLACE_SCENE(opgs16::builtin::sample::SampleGame);
#else
    M_POP_SCENE();
#endif
}

} /// ::opgs16::builtin::sample namespace
