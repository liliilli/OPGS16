#ifndef OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H

/**
 * @file System/Manager/texture_manager.h
 * @brief
 */

#include <memory>
#include <string>
#include <unordered_map>
#include "..\Frame\texture.h"

/**
 * @class TextureManager
 * @brief
 * @date 2018-02-05
 */
class TextureManager final {
public:
	using texture_raw = texture::Texture2D*; // Raw pointer of texture::Texture2D;
	using texture_ptr = std::unique_ptr<std::remove_pointer_t<texture_raw>>; // Unique pointer.
	using texture_map = std::unordered_map<std::string, texture_ptr>;

	static TextureManager& GetInstance() {
		static TextureManager instance{};
		return instance;
	}

	/**
	 * @brief Create and bind texture into container.
	 * If container already have texture with same tag, return nullptr and error set up.
	 *
	 * @param[in] tag
	 * @param[in] texture
	 * @return Raw pointer of texture.
	 */
	texture_raw Create(const std::string& tag, const texture::Texture2D& texture);

	/**
	 * @brief Just create memory space for texture named tag.
	 * In this case, caller must call Initiate() method of texture.
	 * or texture will failed in run time and end with up crashing application.
	 * If container already have texture with same tag, return nullptr and error set up.
	 *
	 * @param[in] tag The tag name of bound texture.
	 * @return Raw pointer of texture.
	 */
	texture_raw Create(const std::string& tag);

	/** TEMPORARY */
	texture_raw TempCreateImage(const std::string& tag, const std::string& image_path,
								const unsigned mode);

	/**
	 * @brief Return bound texture with tag name.
	 * If class did not find bound textrue with tag, return nullptr and error flag set up.
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
	texture_map m_container; // Texture container used in game.

	enum class ErrorType {
		OK,
		/* Errors */
		ALREADY_HAS_INSTANCE,	/* The tag has already a instance, so couldn't create new one. */
	} m_error{ ErrorType::OK };

private:
	/** Check there is already Texture content with name. */
	inline bool IsAlreadyExist(const std::string& tag);

private:
	TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager(const TextureManager&&) = delete;
	TextureManager operator=(const TextureManager&) = delete;
	TextureManager operator=(const TextureManager&&) = delete;
};

inline bool TextureManager::IsAlreadyExist(const std::string& tag) {
	return m_container.find(tag) != m_container.end();
}

#endif /** OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H */