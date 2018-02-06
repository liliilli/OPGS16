#ifndef OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H

/**
 * @file System/Shader/shader_manager.h
 * @brief Shader management class.
 *
 * @author Jongmin Yun
 * @date 2018-02-05
 */

#include <memory>
#include <unordered_map>
#include "shader.h"
#include "..\Frame\object.h"
#include "..\Manager\resource_manager.h"

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
	 * @brief The method returns arbitary shader which has tag name.
	 * If the method fails to search shader has tag name, create shader and return.
	 *
	 * @param[in] name The tag to find.
	 * @return Founded shader pointer, if fails create shader and return late.
	 */
	inline shader_raw GetShaderWithName(const std::string&& name);

	/**
	 * @brief This create new shader return created shader or already existed shader.
	 *
	 * @param[in] tag
	 * @param[in] initializer_list
	 *
	 * @return Shader smart-pointer.
	 */
	using Type = helper::ShaderNew::Type;
	using shader_list = std::initializer_list<std::pair<Type, const std::string&&>>;
	shader_raw CreateShader(const std::string&& tag, shader_list initializer_list);

	using container = ResourceManager::shader_container;
	shader_raw CreateShader(const std::string& tag, const container& list);

	[[noreturn]] void BindObjectToShader(std::string&& name, const Object& object);

	[[noreturn]] void CheckError();

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

/**
 * @brief The method returns arbitary shader which has tag name.
 * If the method fails to search shader has tag name, create shader and return.
 *
 * @param[in] name The tag to find.
 * @return Founded shader pointer, if fails create shader and return late.
 */
inline ShaderManager::shader_raw ShaderManager::GetShaderWithName(const std::string&& name) {
	if (m_shaders.find(name) == m_shaders.end()) {
		// late Initialize
		auto& list = ResourceManager::GetInstance().GetShader(name);
		CreateShader(std::move(name), list);
	}
	return m_shaders.at(name).get();
}

#endif /** OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H */