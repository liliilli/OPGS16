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
 * @file System/Manager/Private/shader_manager.cc
 * @brief ShaderManager implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring. Add copyright and comments.
 */

#include "../Public/shader_manager.h"   /*! Header file */

namespace opgs16 {
namespace manager {

ShaderManager::shader_raw ShaderManager::CreateShader(const std::string& tag,
                                                      const opgs16::resource::SShader& container) {
	if (DoesShaderExist(tag))
        return Shader(tag);

	auto shader = std::make_unique<helper::ShaderNew>();
	for (auto& [type, shader_path] : container.List()) {
		shader->SetShader(type, shader_path.c_str());
	}
	shader->Link();

	/** Bind */
	m_shaders[tag] = std::move(shader);
	return m_shaders[tag].get();
}

void ShaderManager::Clear() {
    //m_shaders.clear();
}

void ShaderManager::ReleaseShader(const std::string& shader_name) {
    if (DoesShaderExist(shader_name))
        m_shaders.erase(shader_name);
}

} /*! opgs16::manager */
} /*! opgs16 */

