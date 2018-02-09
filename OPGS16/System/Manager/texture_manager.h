#ifndef OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H

/**
 * @file System\Manager\texture_manager.h
 * @brief
 *
 * @date 2018-02-06
 */

#include <memory>
#include <string>
#include <unordered_map>
#include "..\Frame\texture.h"

//namespace texture { class Texture2D; }

/**
 * @class TextureManager
 * @brief TextureManager is singleton and can not be a base class of any derived class.
 * TextureManager manages textures which are used by PostProcessing Frame, related to rendering
 * such as sprite_renderer, etc.
 *
 * @date 2018-02-06
 */
class TextureManager final {
public:
	/**
	 * @brief Return single static instance. This must be called in initiation time once.
	 * @return The reference of TextureManager instance.
	 */
	static TextureManager& GetInstance() {
		static TextureManager instance{};
		return instance;
	}

	/** Internal type aliasings */
	using texture_raw = texture::Texture2D*; // Raw pointer of texture::Texture2D;
	using texture_ptr = std::unique_ptr<std::remove_pointer_t<texture_raw>>; // Unique pointer.
	using texture_map = std::unordered_map<std::string, texture_ptr>;

	/**
	 * @brief Return bound texture with tag name.
	 * If class did not find bound texture with tag, create texture2D named tag in ResourceManager.
	 * and return bound texture as a shape of pointer.
	 *
	 * @param[in] tag The tag name of bound texture.
	 * @return Raw pointer of texture.
	 */
	texture_raw GetTexture(const std::string& tag);

	/**
	 * @brief Destory texture and release memory space.
	 * If class did not find bound texture with tag, do nothing and error flag set up.
	 *
	 * @param[in] tag The tag name of bound texture.
	 */
	[[noreturn]] void Release(const std::string& tag);

	/**
	 * @brief
	 */
	[[noreturn]] void CheckError();

private:
	texture_map m_container;	// Texture container used in game.

	enum class ErrorType {
		OK,
		ALREADY_HAS_INSTANCE,	/* The tag has already a instance, so couldn't create new one. */
	} m_error{ ErrorType::OK };

private:
	/** Check there is already Texture content with name. */
	bool IsAlreadyExist(const std::string& tag);

private:
	TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager(const TextureManager&&) = delete;
	TextureManager operator=(const TextureManager&) = delete;
	TextureManager operator=(const TextureManager&&) = delete;
};

#endif /** OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H */