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

/*!
 * @file System/Components/Private/animator.cc
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Create file.
 */

#include "../Public/animator.h" /*! Header file */

namespace opgs16 {
namespace component {

using _internal::AnimatorState;

Animator::Animator(Sprite2DRenderer& bind_renderer, Switch loop) :
    m_renderer{ bind_renderer }, m_loop{ loop }, m_state{ AnimatorState::START } {
    OnStart();
}

void Animator::Update() {
    switch (m_state) {
    default: /*! Do nothing */ break;;
    case AnimatorState::ANIMATION_START: break;
    case AnimatorState::UPDATE: OnUpdate(); break;
    case AnimatorState::ANIMATION_END: break;
    case AnimatorState::END: OnEnd(); break;
    case AnimatorState::SLEEP: break;
    }
}

void Animator::OnStart() {
    m_state = AnimatorState::ANIMATION_START;
}

void Animator::OnAnimationStart() {
    m_state = AnimatorState::UPDATE;
}

void Animator::OnUpdate() {
    /*! Do nothing now */
}

void Animator::OnAnimationEnd() {
    if (IsSwitchOn(m_loop))
        m_state = AnimatorState::UPDATE;
    else
        m_state = AnimatorState::END;
}

void Animator::OnEnd() {
    m_state = AnimatorState::SLEEP;
}

void Animator::OnSleep() {
    /*! Do nothing now */
}

void Animator::OnTriggerTick() {

}

} /*! opgs16::component */
} /*! opgs16 */