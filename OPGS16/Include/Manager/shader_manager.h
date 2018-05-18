#ifndef OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H

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
 * @file System\Shader\shader_manager.h
 * @brief Shader management class.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 * 2018-03-11 Correction of object namespace position and path.
 */

#include <memory>
#include <unordered_map>

/// ::opgs16::manager::resource_manager
#include <Manager\resource_manager.h>
/// ::opgs16::element::CObject
#include <Element\object.h>
/// ::opgs16::element::CShaderNew
#include <Shader\shader.h>

namespace opgs16 {
namespace manager {

/*!
 * @class ShaderManager
 * @brief ShaderManager class manages shaders on runtime in scene.
 *
 * ShaderManager class manages shaders compiled on runtime in scene, These are able to bind
 * the object in use, render binded m_object_list, move binded object to another shader, and be deleted.
 *
 * Shaders in container are managed by shader name, and Tags.
 * ShaderManager is singleton, so user cannot create another ShaderManager instance.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-06 Create file and make basic features.
 * 2018-03-04 Refactoring.
 *
 * @todo Implement ReleaseAll(), ReleaseShader(), CheckError();
 */
class ShaderManager final {
public:
	using shader_raw = element::CShaderNew*;
	using shader_ptr = std::unique_ptr<element::CShaderNew>;
    using shader_map = std::unordered_map<std::string, shader_ptr>;

	/*! Return singleton instance of ShaderManager class. */
	static ShaderManager& Instance() {
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
	inline shader_raw Shader(const std::string& name);

	/** Release all shader program in shader container. */
	void Clear();

	/**
	 * @brief
	 * @param[in] shader_name The name tag to release.
	 */
	void ReleaseShader(const std::string& shader_name);

private:
	/**
	 * @brief Override version method of CreateShader(std::string, shader_list).
	 * @param[in] tag The tag to specify.
	 * @param[in] list vector conatiner contains shader arguments to create.
	 * @return created shader raw-pointer.
	 */
	shader_raw CreateShader(const std::string& tag, const opgs16::resource::SShader& container);

private:
	/** ShaderManager error type */
	enum class ErrorType {
		OK,
	} m_error{ ErrorType::OK };

	shader_map m_shaders{};	// Shader container.

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
	ShaderManager();

public:
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
inline ShaderManager::shader_raw ShaderManager::Shader(const std::string& name) {
	if (!DoesShaderExist(name)) {
		auto& list = MResourceManager::Instance().GetShader(name);
		CreateShader(name, list);
	}
	return m_shaders[name].get();
}

} /*! opgs16::manager */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H */