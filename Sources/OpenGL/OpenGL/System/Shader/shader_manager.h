#ifndef OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H

/**
 * @file System/Shader/shader_manager.h
 * @brief Shader management class.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "shader.h"
#include "..\..\object.h"
#include <memory>

/**
 * @class ShaderManager
 * @brief ShaderManager class manages shaders on runtime in scene.
 *
 * ShaderManager class manages shaders compiled on runtime in scene, These are able to bind
 * the object in use, render binded objects, move binded object to another shader, and be deleted.
 *
 * Shaders in container are managed by shader name, and Tags.
 *
 * ShaderManager is singleton, so user cannot create another ShaderManager instance.
 */
class ShaderManager {
private:
	template<typename _Ty>
	using hash_container = std::unordered_map<std::string, _Ty >> ;

	enum class FailType {
		NONE
	};

public:
	/**
	 * @brief Static method gets unique instance of ShaderManager class.
	 */
	static ShaderManager& GetInstance() {
		static ShaderManager instance{};
		return instance;
	}

	/**
	 * @brief The method returns arbitary shader.
	 */
	helper::ShaderNew& GetShaderWithName(std::string&& name);

	[[noreturn]] void DrawWithShader(std::string&& name);

	[[noreturn]] void BindObjectToShader(std::string&& name, const Object& object);

	bool Fail();

	[[noreturn]] void CleanAll();

	[[noreturn]]
	[[maybe_unused]] void CleanWithTag();

private:
	ShaderManager();

	bool m_failed{ false };

	FailType m_fail_type{ FailType::NONE };

	hash_container<helper::ShaderNew&> m_shaders{};

public:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager(const ShaderManager&&) = delete;
	ShaderManager& operator=(const ShaderManager&&) = delete;
};

#endif /** OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H */