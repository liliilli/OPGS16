#ifndef OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H
#define OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H

/**
 * @file System/Shader/pp_manager.h
 * @brief Elementary manager class to manage post-processing shaders.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <memory>
#include <string>
#include <unordered_map>
#include "pp_frame.h"

namespace shading {

/**
 * @class PostProcessingManager
 */
class PostProcessingManager {
public:
	using pp_effect = std::unique_ptr<shading::PostProcessingFrame>;

public:
	/**
	 * @brief Static method gets unique instance of PostProcessingManager class.
	 */
	static PostProcessingManager& GetInstance() {
		static PostProcessingManager instance{};
		return instance;
	}

	/**
	 * @brief
	 */
	pp_effect& GetEffect(const std::string&& tag);

	/**
	 * @brief
	 */
	bool InsertEffect(const std::string&& tag);

	inline bool IsEffectExist(const std::string tag);

private:
	std::unordered_map<std::string, std::unique_ptr<shading::PostProcessingFrame>> m_effects;
};

inline bool PostProcessingManager::IsEffectExist(const std::string tag) {
	if (m_effects.find(tag) == m_effects.end()) return false;

	return true;
}

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H */