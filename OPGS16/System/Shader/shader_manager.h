#ifndef OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H

/**
 * @file System\Shader\shader_manager.h
 * @brief Shader management class.
 *
 * @author Jongmin Yun
 * @date 2018-02-06
 */

#include <memory>
#include <unordered_map>
#include "shader.h"
#include "..\Object\object.h"
#include "..\Manager\resource_manager.h"

/**
 * @class ShaderManager
 * @brief ShaderManager class manages shaders on runtime in scene.
 *
 * ShaderManager class manages shaders compiled on runtime in scene, These are able to bind
 * the object in use, render binded m_object_list, move binded object to another shader, and be deleted.
 *
 * Shaders in container are managed by shader name, and Tags.
 * ShaderManager is singleton, so user cannot create another ShaderManager instance.
 * @date 2018 - 02 - 06
 * @todo Implement ReleaseAll(), ReleaseShader(), CheckError();
 */
class ShaderManager final {
private:
	template<typename _Ty>
	using hash_container = std::unordered_map<std::string, _Ty>;

public:
	using shader_raw = helper::ShaderNew*;
	using shader_ptr = std::unique_ptr<helper::ShaderNew>;
	using shader_type = helper::ShaderNew::Type;
	using shader_list = std::initializer_list<std::pair<shader_type, const std::string>>;
	using shader_vec = ResourceManager::shader_container;

	/**
	 * @brief Static method gets unique instance of ShaderManager class.
	 * @return The reference of ShaderManager instance.
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
	inline shader_raw GetShaderWithName(const std::string& name);

	/**
	 * @brief This create new shader return created shader or already existed shader.
	 *
	 *
	 * @param[in] tag The tag to specify.
	 * @param[in] initializer_list Shader arugments to create appropriate shader.
	 * @return Shader raw-pointer.
	 */
	shader_raw CreateShader(const std::string& tag, const shader_list& initializer_list);

	/**
	 * @brief Override version method of CreateShader(std::string, shader_list).
	 * @param[in] tag The tag to specify.
	 * @param[in] list vector conatiner contains shader arguments to create.
	 * @return created shader raw-pointer.
	 */
	shader_raw CreateShader(const std::string& tag, const shader_vec& list);

	/**
	 * @brief Release all shaders in shader container.
	 */
	 void Clear();

	/**
	 * @brief
	 * @param[in] shader_name The name tag to release.
	 */
	 void ReleaseShader(const std::string& shader_name);

	/**
	 * @brief
	 */
	 void CheckError();

private:
	/** ShaderManager error type */
	enum class ErrorType {
		OK,
	} m_error{ ErrorType::OK };

	hash_container<shader_ptr> m_shaders{};	// Shader container.

private:
	/**
	 * @brief Check and return true / false whether or not Shader with name is already initiated.
	 * @param[in] shader_name Shader name to search.
	 * @return If shader already exist, return true else false.
	 */
	inline bool DoesShaderExist(const std::string& shader_name) {
		return m_shaders.find(shader_name) != m_shaders.end();
	}

private:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
};

/**
 * @brief The method returns arbitary shader which has tag name.
 * If the method fails to search shader has tag name, create shader and return.
 *
 * @param[in] name The tag to find.
 * @return Founded shader pointer, if fails create shader and return late.
 */
inline ShaderManager::shader_raw ShaderManager::GetShaderWithName(const std::string& name) {
	if (m_shaders.find(name) == m_shaders.end()) {
		// late Initialize
		auto& list = ResourceManager::GetInstance().GetShader(name);
		CreateShader(name, list);
	}
	return m_shaders.at(name).get();
}

#endif /** OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H */