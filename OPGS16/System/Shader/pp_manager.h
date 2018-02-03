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

/**
 * @namespace shading
 * @brief shading namespace is for objects related to shading and shader management.
 */
namespace shading {

/**
 * @class PostProcessingManager
 * @brief This class manages post-processing frame (instances) to be used in Application.
 * Each post-processing instance stored in container could be attached, released, destroyed in
 * any time but Strongly recommends it in Initiate time and Update time.
 */
class PostProcessingManager final {
public:
	using pp_effect = std::unique_ptr<shading::PostProcessingFrame>; // Abbreviation.
	using ppf_ptr	= shading::PostProcessingFrame*;
	using sequence_type = std::list<ppf_ptr>;

public:
	/** Constructor */
	PostProcessingManager();

	/**
	 * @brief Static method gets unique instance of PostProcessingManager class.
	 * @return Lvalue reference of singleton instance.
	 */
	static PostProcessingManager& GetInstance() {
		static PostProcessingManager instance{};
		return instance;
	}

	/**
	 * @brief Get lvalue reference of effect which is named with tag.
	 * @param[in] tag
	 * @return Lvalue reference of effect.
	 */
	pp_effect& GetEffect(const std::string&& tag);

	/**
	 * @brief Insert effect (vacant) instance into effect container.
	 * Type paramter must be written in <> and which must derive PostProcessingFrame base class.
	 *
	 * @param[in] tag tag The tag to use.
	 * @param[in] _Ty Post-processing Effect type parameter to use.
	 * @return If this method success to create and insert _Ty effect, return True.
	 */
	template <class _Ty,
		typename = std::enable_if_t<std::is_base_of_v<shading::PostProcessingFrame, _Ty>>>
	bool InsertEffect(const std::string& tag) {
		if (IsEffectExist(tag)) { return false; }

		m_effects[tag] = std::make_unique<_Ty>();
		return true;
	}

	/** Overloading version of InsertEffect<_Ty, ...>(tag) */
	bool InsertEffect(const std::string& tag);

	/**
	 * @brief Insert effect and initiate automatically.
	 * Type paramter must be written in <> and which must derive PostProcessingFrame base class.
	 *
	 * @param[in] tag tag The tag to use.
	 * @param[in] _Ty Post-processing Effect type parameter to use.
	 * @return If this method success to create and insert _Ty effect, return True.
	 */
	template <class _Ty,
		typename = std::enable_if_t<std::is_base_of_v<shading::PostProcessingFrame, _Ty>>>
	bool InsertEffectInitiate(const std::string& tag) {
		if (InsertEffect<_Ty>(tag)) {
			m_effects[tag]->Initiate();
			return true;
		}
		else return false;
	}

	/**
	 * @brief Find whether or not effect named tag is exist.
	 * @param[in] tag The tag to find effect.
	 * @return If searching effect is successful, return true. else return false.
	 */
	inline bool IsEffectExist(const std::string tag);

	/**
	 * @brief Set continous post-processing sequence to render screen.
	 * If sequence list on id is already set up, This method does nothing.
	 *
	 * @param[in] id Id which is position to be set effect sequences.
	 * @param[in] list Effect tags list
	 * @return Pointer of initialized effect sequence.
	 * If this fails to create sequence, return nullptr.
	 */
	const sequence_type* const
		SetSequence(const size_t id, const std::initializer_list<std::string>& list);

	/**
	 * @brief Updates each effects of each sequences. (each effects of sequence is active)
	 */
	[[noreturn]] void UpdateSequences();

	/**
	 * @brief Bind effect sequence with id number.
	 * @param[in] id Index position of effect sequences container to bind seqeunce.
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

	/**
	 * @brief
	 */
	[[noreturn]] void Render();

private:
	/** Container sotres post-processing separated effects. */
	std::unordered_map<std::string, pp_effect> m_effects;
	/** Container stores post-processing sequences combined with effects. */
	std::unordered_map<size_t, sequence_type> m_effect_sequences;

	const size_t m_reset = 0xffff;				// Just used for reset of m_binded_number
	mutable size_t m_binded_number{m_reset};	// if not reset, call effect sequence via this

private:
	/** Return id'th position of effect_sequences is already exist. */
	inline bool IsEffectSequenceAlreadyExist(const size_t id);
};

// Inline Methods

inline bool PostProcessingManager::IsEffectExist(const std::string tag) {
	if (m_effects.find(tag) == m_effects.end()) return false;
	return true;
}

inline bool PostProcessingManager::IsEffectSequenceAlreadyExist(const size_t id) {
	return m_effect_sequences.find(id) != m_effect_sequences.end();
}

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H */