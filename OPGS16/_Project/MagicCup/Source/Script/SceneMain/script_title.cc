
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_title.h"

#include <Component/sprite2d_renderer.h>
#include <Element/object.h>
#include <Element/canvas/canvas.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Internal/object_keyword.h"
#include "../../../Include/Object/CanvasItem/background.h"

#include "../../../Include/Script/SceneMain/script_component.h"
#include "../../../Include/Script/SceneMain/script_select.h"

namespace {

static constexpr float final_y_pos = 48.f;
static constexpr float height = 160.f;
static constexpr float effect_time = 4.0f;
static constexpr float a = height / effect_time;
static constexpr float freq = 3.1415926535f;

} /// unnamed namespace

namespace magiccup {

void ScriptTitleDisplay::Initiate() {
  auto& bind_obj = GetBindObject();

  using opgs16::element::canvas::CImage;
  using opgs16::element::canvas::CCanvas;

  auto title = bind_obj.CreateGameObject<CImage>(
      name::title, keyword::rsc_sprite,
      static_cast<CCanvas*>(&bind_obj));
  title->SetImageSize(256, 64);
  title->SetRenderingLayer("Interface");
  title->SetTextureFragmentIndex(8);
  title->SetWorldPosition({0, height + final_y_pos, 0});
  m_title = title;
}

void ScriptTitleDisplay::Update(float delta_time) {
  m_elapsed += delta_time;
  if (m_elapsed >= effect_time) {
    m_title->SetWorldPosition({0, final_y_pos, 0});
    this->SetComponentActive(false);
    OP16_TIMER_SET(m_timer, 1'000, false, this, &ScriptTitleDisplay::ShowUpComponents);
  }

  const float new_y_pos =
      (height - (a * m_elapsed)) * abs(std::cosf(freq * m_elapsed));
  m_title->SetWorldPosition({0, new_y_pos + final_y_pos, 0});
}

void ScriptTitleDisplay::ShowUpComponents() {
  auto& obj = GetBindObject();

  auto script_select = obj.GetComponent<ScriptTitleSelect>();
  script_select->EnableComponent();

  auto script_component = obj.GetComponent<ScriptTitleComponent>();
  script_component->EnableComponent();
}

} /// ::magiccup namespace
