
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Script/hopping.cc
///

/// Header file
#include "../../Include/Script/hopping.h"

#include <chrono>
#include <random>

/// ::opgs16::component::CSprite2DRenderer
#include <Component/sprite_renderer.h>
/// ::opgs16::manager::MObjectManager
#include <Manager/object_manager.h>
/// ::opgs16::manager::MTimerManager
#include <Manager/timer_manager.h>

#undef GetObject

constexpr float k_angle_offset = 3.1725f;

namespace {

int32_t Clamp(uint32_t value, uint32_t lhs, uint32_t rhs) {
  return (value % (rhs - lhs)) + lhs;
}

} /// unnamed namespace

namespace debug::script {

void Hopping::Initiate() {
  static std::default_random_engine rng{};
  if (!s_is_rng_initiated) {
    rng.seed(static_cast<std::default_random_engine::result_type>(
        std::chrono::system_clock::now().time_since_epoch().count()
      ));
    s_is_rng_initiated = true;
  }

  m_delay = static_cast<float>(Clamp(static_cast<uint32_t>(rng()), 2, 4));
  m_intensity = static_cast<float>(Clamp(static_cast<uint32_t>(rng()), 100, 120));

  auto x = Clamp(static_cast<uint32_t>(rng()), 32, 256 - 32);
  auto y = Clamp(static_cast<uint32_t>(rng()), 16, 224 - 16);
  auto z = Clamp(static_cast<uint32_t>(rng()), 50, 100);
  m_init_z = -static_cast<float>(z);

  auto& object = GetObject();
  object.SetWorldPosition({
      static_cast<float>(x),
      static_cast<float>(y),
      m_init_z
  });

  using Sprite2DRenderer = opgs16::component::CSprite2DRenderer;
  if (Sprite2DRenderer* renderer = object.GetComponent<Sprite2DRenderer>();
      renderer) {
    renderer->SetRenderLayer(rng() % 4);
  }
}

void Hopping::Start() {
  M_SET_TIMER(m_timer_handler,
              static_cast<long>(m_delay * 1000),
              false,
              this,
              &Hopping::DestroySelf);
}

void Hopping::Update() {
  using EDirection = opgs16::element::_internal::EDirection;
  auto& object = GetObject();

  float angle_value = object.GetRotationLocalAngle(EDirection::Z);
  angle_value += k_angle_offset;
  object.SetRotationLocalAngle(EDirection::Z, angle_value);

  m_elapsed += 0.016f;
  const auto pos = object.GetWorldPosition();
  const auto new_z =
      m_intensity * std::sin(3.14159f / m_delay * m_elapsed) + m_init_z;
  object.SetWorldPosition({pos.x, pos.y, new_z});
}

void Hopping::OnEnabled() {

}

void Hopping::OnDisabled() {

}

void Hopping::Destroy() {

}

void Hopping::DestroySelf() {
  opgs16::manager::object::Destroy(GetObject());
}

} /// ::debug::script