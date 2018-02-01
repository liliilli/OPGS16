#include "pp_manager.h"

namespace shading {

using PPManager = PostProcessingManager;
PPManager::pp_effect& PostProcessingManager::GetEffect(const std::string&& tag) {
	if (IsEffectExist(tag)) return m_effects.at(tag);
}

bool PostProcessingManager::InsertEffect(const std::string& tag) {
	return InsertEffect<shading::PostProcessingFrame>(tag);
}

using pp_effect = std::shared_ptr<shading::PostProcessingFrame>;
const std::list<pp_effect>* const
PostProcessingManager::SetSequence(
	const size_t id, const std::initializer_list<std::string>& list) {
	/** Body */
	if (!IsEffectSequenceAlreadyExist(id)) {
		std::list<pp_effect>&& effect_list{};
		for (const auto& tag : list) {
			if (IsEffectExist(tag)) {
				effect_list.push_back(GetEffect(std::string{ tag }));
			}
			else return nullptr;
		}
		m_effect_sequences[id] = std::move(effect_list);
		return &m_effect_sequences[id];
	}
	else return nullptr;
}

void PostProcessingManager::BindSequence(const size_t id) {
	if (IsEffectSequenceAlreadyExist(id)) {
		m_binded_number = id;
		/** Bind first */
		(*m_effect_sequences.at(id).begin())->Bind();
	}
}

void PostProcessingManager::RenderSequence() {
	if (m_binded_number != m_reset) {
		auto& list = m_effect_sequences.at(m_binded_number);
		/** Iterate effect list, and rendering */
		auto it_effect = list.begin();
		auto it_rend = (--list.end());

		std::for_each(list.begin(), list.end(), [&](decltype(*it_effect)& effect) {
			if (it_effect != it_rend) /** If this is not end of sequence, bind next. */ {
				(*std::next(it_effect))->Bind();
			}
			else { /** Bind to default frame buffer */
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			/** Render! */
			effect->RenderEffect();
			it_effect++;
		});
	}
}

void PostProcessingManager::ReleaseSequence(const size_t id) {
	if (m_effect_sequences.find(id) != m_effect_sequences.end()) {
		if (m_binded_number == id) m_binded_number = m_reset;
		m_effect_sequences.erase(id);
	}
	else {
		// Do something.
	}
}

};