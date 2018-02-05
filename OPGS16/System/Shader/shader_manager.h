#ifndef OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H

/**
 * @file System/Shader/shader_manager.h
 * @brief Shader management class.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <memory>
#include <unordered_map>
#include "shader.h"
#include "..\Frame\object.h"

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
class ShaderManager final {
private:
	template<typename _Ty>
	using hash_container = std::unordered_map<std::string, _Ty>;

public:
	using shader_raw = helper::ShaderNew*;
	using shader_ptr = std::unique_ptr<helper::ShaderNew>;

	/**
	 * @brief Static method gets unique instance of ShaderManager class.
	 */
	static ShaderManager& GetInstance() {
		static ShaderManager instance{};
		return instance;
	}

	/**
	 * @brief The method returns arbitary shader.
	 * @param[in] name
	 */
	inline shader_raw GetShaderWithName(const std::string&& name) const {
		if (m_shaders.find(name) == m_shaders.end()) return nullptr;
		return m_shaders.at(name).get();
	}

	/**
	 * @brief This create new shader return created shader or already existed shader.
	 *
	 * @param[in] tag
	 * @param[in] initializer_list
	 *
	 * @return Shader smart-pointer.
	 */
	using Type = helper::ShaderNew::Type;
	shader_raw CreateShader(const std::string&& tag,
		std::initializer_list<std::pair<Type, const std::string&&>> initializer_list);

	[[noreturn]] void DrawWithShader(std::string&& name);

	[[noreturn]] void BindObjectToShader(std::string&& name, const Object& object);

	bool Fail();

	[[noreturn]] void CleanAll();

	[[noreturn]] void CleanWithTag();

private:
	enum class ErrorType {
		OK,
	} m_error{ ErrorType::OK };

	hash_container<shader_ptr> m_shaders{};	// Shader container.

public:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager(const ShaderManager&&) = delete;
	ShaderManager& operator=(const ShaderManager&&) = delete;
};

#endif /** OPENGL_TUTORIAL_SYSTEM_SHADER_SHADER_MANAGER_H */