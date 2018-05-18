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
 * @file System/Manager/Private/postprocessing_manager.cc
 * @brief
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 */

#include <string_view>                                  /*! std::string_view */

#include "../Public/postprocessing_manager.h"                       /*! Header file */
#include "../../Shader/PostProcessing/pp_scaling.h"

namespace opgs16 {
namespace manager {

namespace {

using PPManager = MPostProcessingManager;
using sequence_type = MPostProcessingManager::sequence_type;

using namespace std::string_view_literals;
constexpr std::string_view scale_effect{ "__df__"sv };

} /*! unnamed namespace */

MPostProcessingManager::MPostProcessingManager() {
    /*! Insert display size scaling post processing effect */
	InsertEffectInitiate<builtin::postprocessing::PpEffectScaling>("__df__");
    SetSequence(0u, { });
}

PPManager::pp_effect& MPostProcessingManager::GetEffect(const std::string&& tag) {
	if (IsEffectExist(tag))
        return m_effects[tag];
}

bool MPostProcessingManager::InsertEffect(const char* tag) {
	return InsertEffect<element::CPostProcessingFrame>(tag);
}

const sequence_type* PPManager::SetSequence(const size_t id, const std::initializer_list<std::string>& list) {
	if (!DoesEffectSequenceExist(id)) {
		std::list<ppf_ptr>&& effect_list{};
		for (const auto& tag : list) {
			if (IsEffectExist(tag)) {   /*! Push and get count up. */
				effect_list.emplace_back(GetEffect(std::string{ tag }).get());
				(*effect_list.rbegin())->Active();
			}
			else return nullptr;
		}
        /*! Insert scaling post-processing effect, for default displaying. Not have to active() */
        effect_list.emplace_back(GetEffect("__df__").get());

		// Insert
		m_effect_sequences[id] = std::move(effect_list);
		return &m_effect_sequences[id];
	}
	else return nullptr;
}

void MPostProcessingManager::UpdateSequences() {
	for (auto& effect : m_effects) {
		auto& item = effect.second;
		if (item->IsActive()) item->Update();
	}
}

void MPostProcessingManager::BindSequence(const size_t id) {
	if (DoesEffectSequenceExist(id)) {
		m_binded_number = id;

		/*! Bind first effect */
		(*m_effect_sequences.at(id).begin())->Bind();
	}
}

void MPostProcessingManager::Render() {
    /*! If any of effect sequence is bound, render post-processing */
	if (m_binded_number != m_reset) {
		auto& list = m_effect_sequences.at(m_binded_number);
		/** Iterate effect list, and rendering */
		auto it_effect = list.begin();
		auto it_rbegin = (--list.end());

		std::for_each(list.begin(), list.end(), [&](decltype(*it_effect)& effect) {
			if (it_effect != it_rbegin) /** If this is not end of sequence, bind next. */ {
				(*std::next(it_effect))->Bind();
			}
			else { /** Bind to default frame buffer. */
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			/** Render! */
			effect->RenderEffect();
            ++it_effect;
		});
	}
}

void MPostProcessingManager::ReleaseSequence(const size_t id) {
	if (DoesEffectSequenceExist(id)) {
		if (m_binded_number == id) m_binded_number = m_reset;

		// Get count down and erase sequences.
		for (auto& item : m_effect_sequences[id]) {
			item->Disable();
		}
		m_effect_sequences.erase(id);
	}
	else {
		// Do something.
	}
}

} /*! opgs16::manager */
} /*! opgs16 */
