#ifndef OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H
#define OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H

/**
 * @file System/Shader/pp_manager.h
 * @brief Elementary manager class to manage post-processing shaders.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.2
 */

#include <memory>
#include <string>
#include <unordered_map>
#include "pp_frame.h"

namespace shading {

/**
 * @class PostProcessingManager
 * @brief
 */
class PostProcessingManager {
public:
	using pp_effect = std::shared_ptr<shading::PostProcessingFrame>; /** Abbreviation */

public:
	/**
	 * @brief Static method gets unique instance of PostProcessingManager class.
	 * @return
	 */
	static PostProcessingManager& GetInstance() {
		static PostProcessingManager instance{};
		return instance;
	}

	/**
	 * @brief
	 * @param[in] tag
	 * @return
	 */
	pp_effect& GetEffect(const std::string&& tag);

	/**
	 * @brief
	 * @param[in] tag
	 * @return
	 */
	bool InsertEffect(const std::string&& tag);

	/**
	 * @brief
	 * @param[in] tag
	 * @return
	 */
	inline bool IsEffectExist(const std::string tag);

	/**
	 * @brief Set continous post-processing sequence to render screen.
	 * If sequence list on id is already set up, This method does nothing.
	 *
	 * @param[in] id
	 * @param[in] list
	 * @return Pointer
	 */
	const std::list<pp_effect>* const
		SetSequence(const size_t id, const std::initializer_list<std::string>& list);

	/**
	 * @brief
	 * @param[in] id
	 */
	[[noreturn]] void BindSequence(const size_t id);

	/**
	 * @brief
	 */
	[[noreturn]] void RenderSequence();

	/**
	 * @brief
	 * #param[in] list
	 */
	[[noreturn]] void ReleaseSequence(const size_t id);

private:
	/** Container sotres post-processing separated effects. */
	std::unordered_map<std::string, pp_effect> m_effects;
	/** Container stores post-processing sequences combined with effects. */
	std::unordered_map<size_t, std::list<pp_effect>> m_effect_sequences{};

	const size_t m_reset = 0xffff;				/** Just used for reset of m_binded_number */
	mutable size_t m_binded_number{m_reset};	/** if not reset, call effect sequence via this */

private:
	/** Return id'th position of effect_sequences is already exist. */
	inline bool IsEffectSequenceAlreadyExist(const size_t id);
};

/** Inline Methods */

inline bool PostProcessingManager::IsEffectExist(const std::string tag) {
	if (m_effects.find(tag) == m_effects.end()) return false;
	return true;
}

inline bool PostProcessingManager::IsEffectSequenceAlreadyExist(const size_t id) {
	return m_effect_sequences.find(id) != m_effect_sequences.end();
}

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H */