#include "pp_manager.h"

namespace shading {

using PPManager = PostProcessingManager;
PPManager::pp_effect& PostProcessingManager::GetEffect(const std::string&& tag) {
	if (IsEffectExist(tag)) return m_effects.at(tag);
}

bool PostProcessingManager::InsertEffect(const std::string && tag) {
	if (IsEffectExist(tag)) {
		return false;
	}

	m_effects[tag] = std::make_unique<shading::PostProcessingFrame>();
}

};