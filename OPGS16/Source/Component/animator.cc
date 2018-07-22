
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Components/animator.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-10 Create file.
/// 2018-04-06 Replace two-index with one-index as ReadFile::index.
///

#include <string>

#include <Component\animator.h> /// Header file

#include <Component\sprite2d_renderer.h>  /// opgs16::component::CSprite2DRenderer
#include <Element\object.h>             /// ::opgs16::element::CObject
#include <Manager\resource_manager.h>   /// ::opgs16::manager::MResourceManager
#include <Manager\resource_type.h>      /// resource::STexture2D::IndexSize

namespace opgs16 {
namespace component {
namespace {
using Object = element::CObject;
using _internal::AnimatorState;
} /*! unnamed namespace */

CAnimator::CAnimator(Object& bind_object, CSprite2DRenderer& bind_renderer,
                     const std::string& load_name, bool is_loop) :
    _internal::CComponent{ bind_object },
    m_renderer{ bind_renderer },
    m_is_loop{ is_loop ? Switch::ON : Switch::OFF },
    m_state{ AnimatorState::UPDATE } {

  const resource::SAnimation* container = manager::resource::GetAnimation(load_name);

  if (container) {
    m_animation   = container;
    m_cell_index  = 0;
    m_cell_length = static_cast<int32_t>(container->cells.size());
    m_cell_time   = static_cast<float>(m_animation->cells[m_cell_index].m_time_milli);
  }
}

void CAnimator::Update(float delta_time) {
  switch (m_state) {
  default: /*! Do nothing */ break;;
  case AnimatorState::UPDATE: OnUpdate(delta_time); break;
  case AnimatorState::SLEEP: break;
  }
}

bool CAnimator::IsSleep() const noexcept {
  return m_state == AnimatorState::SLEEP;
}

void CAnimator::OnUpdate(float delta_time) {
  m_elapsed_time += delta_time * 1'000;
  if (m_elapsed_time < m_cell_time) return;

  m_elapsed_time = 0.f;
  if ((++m_cell_index) < m_cell_length) {
    SetNextAnimationCell();
    return;
  }

  OnAnimationEnd();
  if (m_state == AnimatorState::UPDATE) {
    m_cell_index = 0;
    SetNextAnimationCell();
  }
}

void CAnimator::SetNextAnimationCell() {
  m_renderer.SetTexture(m_animation->cells[m_cell_index].m_texture_name);
  m_renderer.SetTextureFragmentIndex(m_animation->cells[m_cell_index].m_fragment_index);

  m_cell_time = static_cast<float>(m_animation->cells[m_cell_index].m_time_milli);
}

void CAnimator::OnAnimationEnd() {
  if (IsSwitchOn(m_is_loop))
    m_state = AnimatorState::UPDATE;
  else
    m_state = AnimatorState::SLEEP;
}

void CAnimator::OnSleep() { }

} /*! opgs16::component */
} /*! opgs16 */