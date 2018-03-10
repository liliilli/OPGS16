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

#include <fstream>
#include <sstream>
#include <string>

#include "../Public/animator.h"                     /*! Header file */
#include "../../Components/Public/sprite_renderer.h"/*! opgs16::component::Sprite2DRenderer */
#include "../../Manager/Public/resource_type.h"     /*! resource::Texture2D::IndexSize */
#include "../../Manager/Public/texture_manager.h"   /*! opgs16::manager::TextureManager */
#include "../../Manager/Public/timer_manager.h"     /*! TimerManager */

namespace opgs16 {
namespace component {

namespace {

void ReadFile(const char* file_path, std::vector<_internal::AnimationCell>& container) {
    std::ifstream file_stream{ file_path, std::ios::in };
    if (file_stream.good()) {
        std::string line;
        while (std::getline(file_stream, line)) {
            if (line.empty() || line[0] == '#') continue; /*! Empty line || Continue */
            std::stringstream line_stream{ line };

            std::string texture2d_name; unsigned index_x, index_y, time_milli;
            line_stream >> texture2d_name >> index_x >> index_y >> time_milli;

            using IndexSize = opgs16::resource::Texture2D::IndexSize;
            if (auto raw = manager::TextureManager::Instance().GetTexture(texture2d_name); raw) {
                container.emplace_back(raw->GetId(),
                                       IndexSize{ index_x, index_y },
                                       time_milli);
            } else throw new std::runtime_error("there is no texture_2d.");
        }
    }
}

} /*! unnamed namespace */

using _internal::AnimatorState;

Animator::Animator(Sprite2DRenderer& bind_renderer, Switch loop) :
    m_renderer{ bind_renderer }, m_loop{ loop }, m_state{ AnimatorState::START } {
    OnStart();

    ReadFile("_Project/Maintenance/Resource/Animation/test.anim", m_cells);
    if (!m_cells.empty()) {
        OnAnimationStart();
        m_cell_index = 0;
        m_cell_length = m_cells.size();

        using manager::TimerManager;
        TimerManager::Instance().SetTimer(m_timer,
                                          static_cast<long>(m_cells[m_cell_index].TimeMilli()),
                                          false, this, &Animator::OnTriggerTick);
    }
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
    if ((++m_cell_index) != m_cell_length) {
        //m_renderer.SetTextureId(m_cells[m_cell_index].TextureId());
        m_renderer.SetTextureIndex(m_cells[m_cell_index].Index());

        using manager::TimerManager;
        TimerManager::Instance().SetTimer(m_timer,
                                          static_cast<long>(m_cells[m_cell_index].TimeMilli()),
                                          false, this, &Animator::OnTriggerTick);
    }
}

} /*! opgs16::component */
} /*! opgs16 */