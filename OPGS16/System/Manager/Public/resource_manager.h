#ifndef OPGS16_MANAGER_RESOURCE_MANAGER_H
#define OPGS16_MANAGER_RESOURCE_MANAGER_H

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

/*!
 * @file System\Manager\resource_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-27 Correction of loading mechanism
 * 2018-02-28 Corrected GetTexture2D return type must be resource::Texture2D structure.
 * 2018-03-02 Move ResourceManager class inside opgs16::manager namespace for uniformation.
 * 2018-03-03 Refactored and removed member functions are not appropriate for class.
 */

#include <string>				/** std::string */
#include <unordered_map>		/** std::unordered_map */

#include "resource_type.h"      /*! resource:: */
#include "../../../Headers/Fwd/objectfwd.h"

namespace opgs16 {
/*! Namespace for manager */
namespace manager {

/*!
 * @class ResourceManager
 * @brief This class is singleton and not derivable as derived class.
 * ResourceManager has a rule of managing resource files path, each shader path,
 * , error statement path and so on.
 *
 * ResrouceManager has to be called (invoked) when game application is setting up, before
 * actual game update has been begun.
 *
 * @log
 * 2018-02-27 Correction of loading mechanism
 * 2018-02-28 Corrected GetTexture2D return type must be resource::Texture2D structure.
 * 2018-03-02 Move to opgs16::manager namespace.
 * 2018-03-03 Move private functions which don't match class to non-member function.
 *
 * @todo
 * 2018-02-28 CheckError method must have a procedure to bring error message display call to logger.
 */
class ResourceManager final {
public:
    /*! Resource map list */
    using texture_map       = std::unordered_map<std::string, resource::Texture2D>;
    using sound_map         = std::unordered_map<std::string, resource::SSound>;
    using shader_map        = std::unordered_map<std::string, resource::SShader>;
    using font_map          = std::unordered_map<std::string, resource::SFont>;

public:
	/*! Return single static instance. This must be called in initiation time once. */
	static ResourceManager& Instance() {
		static ResourceManager instance{};
		return instance;
	}

    /*! Empty already allocated resources and load resource with file path. */
    bool ReadResourceFile(const std::string& path);

	/**
	 * @brief Get specific shader list.
	 * @param[in] name_key shader program aliasing name.
	 * @return Specific shader container.
	 */
	const resource::SShader& GetShader(const std::string& name_key);

	/**
	 * @brief Get specific texture path.
	 * @param[in] name_key Wrapping tag name of actual sprite path.
	 * @return Selected 2D texture path.
	 */
	const resource::Texture2D& GetTexture2D(const std::string& name_key);

    /*!
     * @brief Get specified sound path information.
     * @param[in] name_key Wrapping tag name of actual sound path.
     * @return Sound path
     */
    const resource::SSound& GetSound(const std::string& name_key);

    /*!
     * @brief Return font information with success flag.
     * @param[in] name_key Name tag of font information you want to find
     * @return success flag, and information
     */
    std::pair<bool, const resource::SFont*> GetFont(const std::string& name_key);

	/*! Check error caused by processing in this class instance. */
	void CheckError();

private:
	texture_map m_textures;         /*! Texture string container */
	shader_map  m_shaders;          /*! Shader arguments container */
    sound_map   m_sounds;           /*! Sounds path string container */
    font_map    m_fonts;            /*! Font path information container */

	/** Used for checking error invoked when this methods process something. */
	enum class ErrorType {
		OK,							/** Default type, everything is ok. */
		FAILED_INITIALIZE_SHADER,	/** Invoked when failed to input shader information. */
		FAILED_INITIALIZE_TEXTURE2D,/** Invoked when failed to input texture information. */
	} m_error{ ErrorType::OK };

private:
    /*! Read resource along with resource_type */
    void ReadResource(const std::string& token_line, std::ifstream& stream,
                      const std::string& global_path, resource::_internal::EResourceType type);

	void PushShader(const std::string& name_key, const resource::SShader& list);
	void PushTexture2D(const std::string& name_key, const resource::Texture2D& container);
    void PushSound(const std::string& name_key, const resource::SSound& container);
    void PushFont(const std::string& name_key, const resource::SFont& container);

	inline bool ExistShaderKey(const std::string& name_key) const {
		return m_shaders.find(name_key) != m_shaders.end();
	}
	inline bool ExistTextureKey(const std::string& name_key) const {
		return m_textures.find(name_key) != m_textures.end();
	}
    inline bool ExistSoundKey(const std::string& name_key) const {
        return m_sounds.find(name_key) != m_sounds.end();
    }
    inline bool ExistFontKey(const std::string& name_key) const {
        return m_fonts.find(name_key) != m_fonts.end();
    }

private:
    ResourceManager() = default;

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
};

} /*! opgs16::manager */

} /*! opgs16 */

#endif /** OPGS16_MANAGER_RESOURCE_MANAGER_H */