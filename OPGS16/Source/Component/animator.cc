/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/// @file Components/Private/animator.cc
/// @author Jongmin Yun
/// @log
/// 2018-03-10 Create file.
/// 2018-04-06 Replace two-index with one-index as ReadFile::index.

#include <string>

#include <Component\animator.h> /// Header file

#include <Component\sprite_renderer.h>  /// opgs16::component::CSprite2DRenderer
#include <Element\object.h>             /// ::opgs16::element::CObject
#include <Manager\resource_manager.h>   /// ::opgs16::manager::MResourceManager
#include <Manager\resource_type.h>      /// resource::STexture2D::IndexSize
#include <Manager\timer_manager.h>      /// ::opgs16::manager::MTimerManager

namespace opgs16 {
namespace component {
namespace {
using Object = element::CObject;
using _internal::AnimatorState;
} /*! unnamed namespace */

CAnimator::CAnimator(Object& bind_object, CSprite2DRenderer& bind_renderer,
                     const std::string& load_name, Switch loop) :
    _internal::CComponent{ bind_object },
    m_renderer{ bind_renderer }, m_loop{ loop }, m_state{ AnimatorState::START } {
    OnStart();

    const resource::SAnimation* container = manager::MResourceManager::Instance().GetAnimation(load_name);
    if (container) {
        m_animation = container;
        OnAnimationStart();
        m_cell_index = 0;
        m_cell_length = container->cells.size();

        using manager::MTimerManager;
        MTimerManager::Instance().SetTimer(m_timer,
                                           static_cast<long>(m_animation->cells[m_cell_index].m_time_milli),
                                           false, this, &CAnimator::OnTriggerTick);
    }
}

void CAnimator::Update() {
    switch (m_state) {
    default: /*! Do nothing */ break;;
    case AnimatorState::ANIMATION_START: break;
    case AnimatorState::UPDATE: OnUpdate(); break;
    case AnimatorState::ANIMATION_END: break;
    case AnimatorState::END: OnEnd(); break;
    case AnimatorState::SLEEP: break;
    }
}

void CAnimator::OnStart() {
    m_state = AnimatorState::ANIMATION_START;
}

void CAnimator::OnAnimationStart() {
    m_state = AnimatorState::UPDATE;
}

void CAnimator::OnUpdate() {
    /*! Do nothing now */
}

void CAnimator::OnAnimationEnd() {
    if (IsSwitchOn(m_loop))
        m_state = AnimatorState::UPDATE;
    else
        m_state = AnimatorState::END;
}

void CAnimator::OnEnd() {
    m_state = AnimatorState::SLEEP;
}

void CAnimator::OnSleep() {
    /*! Do nothing now */
}

void CAnimator::OnTriggerTick() {
    if ((++m_cell_index) < m_cell_length) {
        m_renderer.SetTexture(m_animation->cells[m_cell_index].m_texture_name);
        m_renderer.SetTextureIndex(m_animation->cells[m_cell_index].m_fragment_index);

        using manager::MTimerManager;
        MTimerManager::Instance().SetTimer(m_timer,
                                   static_cast<long>(m_animation->cells[m_cell_index].m_time_milli),
                                   false, this, &CAnimator::OnTriggerTick);
    }
    else if (IsSwitchOn(m_loop)) {
        m_cell_index = 0;
        m_renderer.SetTexture(m_animation->cells[m_cell_index].m_texture_name);
        m_renderer.SetTextureIndex(m_animation->cells[m_cell_index].m_fragment_index);

        using manager::MTimerManager;
        MTimerManager::Instance().SetTimer(m_timer,
                                   static_cast<long>(m_animation->cells[m_cell_index].m_time_milli),
                                   false, this, &CAnimator::OnTriggerTick);
    }
}

} /*! opgs16::component */
} /*! opgs16 */