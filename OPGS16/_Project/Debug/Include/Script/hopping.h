#ifndef DEBUG_SCRIPT_HOPPING_H
#define DEBUG_SCRIPT_HOPPING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Script/hopping.h
///

#include <Component/script_frame.h>
/// ::opgs16::element::CTimerHandle
#include <Frame/timer_handle.h>

namespace debug::script {

class Hopping final : public opgs16::component::CScriptFrame {
public:
  Hopping(opgs16::element::CObject& bind_object) :
      CScriptFrame { bind_object } {};

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;
  void OnEnabled() override final;
  void OnDisabled() override final;
  void Destroy() override final;

  void DestroySelf();

  inline static bool s_is_rng_initiated = false;

  float m_delay = 0.0f;
  float m_elapsed = 0.0f;
  float m_init_z = 0.0f;
  float m_intensity = 0.0f;

  opgs16::element::CTimerHandle m_timer_handler;

SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, Hopping)
};

} /// ::debug::script::Hopping

#endif /// DEBUG_SCRIPT_HOPPING_H

