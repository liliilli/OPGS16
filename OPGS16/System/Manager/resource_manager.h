#ifndef OPGS16_MANAGER_RESOURCE_MANAGER_H
#define OPGS16_MANAGER_RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include "..\Shader\shader.h"

class ResourceManager final {
public:
	/**
	 * @brief Return single static instance. This must be called in initiation time once.
	 * @return The reference of ResourceManager instance.
	 */
	static ResourceManager& GetInstance() {
		static ResourceManager instance{};
		return instance;
	}

	using shader_type = helper::ShaderNew::Type;
	using shader_pair = std::pair<shader_type, const std::string>;
	using shader_list = std::initializer_list<shader_pair>;
	using shader_container = std::vector<shader_pair>;

	/**
	 * @brief In initiation time of game application, push shader information to container.
	 * This method must be called at initiation time. otherwise method refuse processing.
	 * If name_key is duplicated, initiation halts and exit game with message.
	 *
	 * param[in] name_key
	 * param[in] list
	 */
	[[noreturn]] void PushShader(const std::string& name_key, const shader_list& list);

	/**
	 * @brief Get specific shader list.
	 * @param[in] name_key
	 * @return Specific shader container.
	 */
	shader_container& GetShader(const std::string& name_key);

	/**
	 * @brief In initialization time of game application, insert texture path information
	 * to container. This method must be called at initialzation time. otherwise method refuse
	 * processing.
	 * If name_key is duplicated, initialization halts and exit game with a message.
	 *
	 * param[in] name_key sprite path's wrapping name.
	 * param[in] path String path to read file when called.
	 */
	[[noreturn]] void PushTexture2D(const std::string& name_key, const std::string& path);

	/**
	 * @brief Get specific texture path.
	 * @param[in] name_key Wrapping tag name of actual sprite path.
	 * @return Selected 2D texture path.
	 */
	const std::string& GetTexture2D(const std::string& name_key);

private:
	/** Texture string container */
	std::unordered_map<std::string, std::string> m_textures;
	/** Shader arguments container */
	std::unordered_map<std::string, shader_container> m_shaders;

private:
	/**
	 * @brief Check whether or not shader has name_key already exists.
	 * In runtime game mechanism, this must not be happened but if happened, you have to make
	 * an log in debug mode.
	 * @param[in] name_key The key to find.
	 * @return boolean value, If duplicated return true else false.
	 */
	inline bool ExistShaderKey(const std::string& name_key) {
		return m_shaders.find(name_key) != m_shaders.end();
	}

	/**
	 * @brief Check whether or not texture has name_key already exists.
	 * In runtime game mechanism, this must not be happened but if happened, you have to make
	 * an log in debug mode.
	 * @param[in] name_key The key to find.
	 * @return boolean value, If exist return true else false.
	 */
	inline bool ExistTextureKey(const std::string& name_key) {
		return m_textures.find(name_key) != m_textures.end();
	}

private:
	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
	ResourceManager operator=(const ResourceManager&) = delete;
	ResourceManager operator=(const ResourceManager&&) = delete;
};

#endif /** OPGS16_MANAGER_RESOURCE_MANAGER_H */