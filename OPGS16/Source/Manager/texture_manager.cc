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
 * @file System/Manager/Private/texture_manager.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-04 Refactoring.
 */

#include <Manager\texture_manager.h>    /// Header file

/// ::opgs16::texture::CTexture2D
#include <Frame\texture.h>
/// ::opgs16::manager::MResourceManager
#include <Manager\resource_manager.h>

namespace opgs16::manager {

TextureManager::texture_raw TextureManager::GetTexture(const std::string& name) {
	if (!DoesTextureExist(name)) {
		const auto container = manager::resource::GetTexture2D(name);

    auto [it, good] =
      m_container.emplace(name, std::make_unique<texture::CTexture2D>(*container));
    if (good)
      return it->second.get();

    return nullptr;
	}
    return m_container[name].get();
}

void TextureManager::Release(const std::string& tag) {
	if (DoesTextureExist(tag)) m_container.erase(tag);
}

void TextureManager::Clear() {
    m_container.clear();
}

} /*! opgs16::manager */
