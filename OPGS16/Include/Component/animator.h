#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H

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
 * @file System/Components/Public/animator.h
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Create file.
 * 2018-03-11 Corection of ::opgs16::element::CObject class.
 */

/// ::opgs16::component::_internal::AnimatorState
#include <Component\Internal\animator_internal.h> 
/// ::opgs16::component::_internal::CComponent
#include <Component\Internal\component.h>
#include <Component\Internal\component_macro.h> /// Component Macroes
#include <Frame\timer_handle.h> /// ::opgs16::element::CTimerHandle
#include <Helper\switch.h>      /// ::opgs16::CSwitch
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
namespace component {

/*!
 * @class CAnimator
 * @brief Animator class for object.
 */
class CAnimator final : public _internal::CComponent {
public:
    explicit CAnimator(element::CObject& bind_object, CSprite2DRenderer& bind_renderer,
                       const std::string& load_name, Switch loop = Switch::OFF);

    /*! Update animator each frame. */
    virtual void Update() override final;

private:
    CSprite2DRenderer& m_renderer;
    Switch m_loop;

    element::CTimerHandle m_timer;
    _internal::AnimatorState m_state;

    const resource::SAnimation* m_animation{ nullptr };
    size_t m_cell_length;
    size_t m_cell_index{};

private:
    void OnStart();

    void OnAnimationStart();

    void OnUpdate();

    void OnAnimationEnd();

    void OnEnd();

    void OnSleep();

    /*! Only used for timer trigger callback. */
    void OnTriggerTick();

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CAnimator)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_PUBLIC_ANIMATOR_H

