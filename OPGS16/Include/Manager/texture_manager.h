#ifndef OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file System/Manager/Public/texture_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-23 TextureManager refactoring.
 * 2018-02-26 Add function 'Clear()'.
 * 2018-02-28 Made it makes Texture2D using resource::Texture2D, not plain path.
 * 2018-03-04 Refactoring.
 */

#include <memory>                   /*! std::unique_ptr<> */
#include <string>                   /*! std::string */
#include <unordered_map>            /*! std::unordered_map */

#include "../../Frame/texture.h"    /*! m_texture::CTexture2D */

namespace opgs16 {
namespace manager {

/**
 * @class TextureManager
 * @brief TextureManager is singleton and can not be a base class of any derived class.
 * TextureManager manages textures which are used by PostProcessing Frame, related to rendering
 * such as sprite_renderer, etc.
 *
 * @log
 * 2018-02-23 TextureManager refactoring.
 * 2018-02-26 Add function 'Clear()'.
 * 2018-03-04 Refactoring. Move TextureManager to opgs16::manager.
 */
class TextureManager final {
private:
	using texture_raw = texture::CTexture2D*;    /*! Raw pointer of m_texture::CTexture2D */
	using texture_ptr = std::unique_ptr<texture::CTexture2D>;            /*! Unique pointer. */
	using texture_map = std::unordered_map<std::string, texture_ptr>;   /*! m_texture list */

public:
	/**
	 * @brief Return single static instance. This must be called in initiation time once.
	 * @return The reference of TextureManager instance.
	 */
	static TextureManager& Instance() {
		static TextureManager instance{};
		return instance;
	}

	/**
	 * @brief Return bound m_texture with tag name.
	 * If class did not find bound m_texture with tag, create texture2D named tag in ResourceManager.
	 * and return bound m_texture as a shape of pointer.
	 *
	 * @param[in] tag The tag name of bound m_texture.
	 * @return Raw pointer of m_texture.
	 */
	texture_raw GetTexture(const std::string& name);

	/**
	 * @brief Destory m_texture and release memory space.
	 * If class did not find bound m_texture with tag, do nothing and error flag set up.
	 *
	 * @param[in] tag The tag name of bound m_texture.
	 */
	 void Release(const std::string& tag);

     /*! Release all resource and clear. */
     void Clear();

private:
	texture_map m_container;	/*! Texture container used in game. */

	enum class ErrorType {
		OK,
		ALREADY_HAS_INSTANCE,	/* The tag has already a instance, so couldn't create new one. */
	} m_error{ ErrorType::OK };

private:
	/** Check there is already Texture content with name. */
    inline bool DoesTextureExist(const std::string& tag) const {
        return m_container.find(tag) != m_container.end();
    }

	TextureManager() = default;

public:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H */