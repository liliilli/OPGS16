#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/animator.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-10 Create file.
/// 2018-03-11 Corection of ::opgs16::element::CObject class.
/// 2018-07-21 Refactoring.
///

/// ::opgs16::component::_internal::AnimatorState
#include <Component/Internal/animator_internal.h>
/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h> /// Component Macroes
#include <Helper/switch.h>      /// ::opgs16::CSwitch
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16::component {

///
/// @class CAnimator
/// @brief Animator class for object.
///
class CAnimator final : public _internal::CComponent {
public:
  explicit CAnimator(element::CObject& bind_object,
                     CSprite2DRenderer& bind_renderer,
                     const std::string& load_name,
                     bool is_loop = false);

  void Update(float delta_time) override final;
  bool IsSleep() const noexcept;

private:
  void OnUpdate(float delta_time);
  void OnAnimationEnd();
  void OnSleep();

  void SetNextAnimationCell();

  CSprite2DRenderer& m_renderer;
  Switch  m_is_loop;

  _internal::AnimatorState m_state;
  const resource::SAnimation* m_animation = nullptr;

  int32_t m_cell_length = 0;
  int32_t m_cell_index = 0;

  float m_elapsed_time = 0.f;
  float m_cell_time = 0.f;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CAnimator)
};

} /// ::opgs16::component namespace

#endif /// !OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H

