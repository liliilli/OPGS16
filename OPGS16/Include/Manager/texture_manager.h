#ifndef OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/texture_manager.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-23 TextureManager refactoring.
/// 2018-02-26 Add function 'Clear()'.
/// 2018-02-28 Made it makes Texture2D using resource::Texture2D, not plain path.
/// 2018-03-04 Refactoring.
/// 2018-07-29 Refactoring. Move class to namespace.
///

#include <memory>
#include <string>
#include <unordered_map>

#include <Element/Internal/texture2d_sprite.h>

///
/// @namespace texture
/// @brief TextureManager is singleton and can not be a base class of any derived class.
/// TextureManager manages textures which are used by PostProcessing Frame, related to rendering
/// such as sprite_renderer, etc.
///
namespace opgs16::manager::texture {
using TTextureRaw    = opgs16::texture::CTexture2DSprite*;
using TTextureSmtPtr = std::unique_ptr<opgs16::texture::CTexture2DSprite>;
using TTextureMap    = std::unordered_map<std::string, TTextureSmtPtr>;

///
/// @brief Return bound m_texture with tag name.
/// If class did not find bound m_texture with tag, create texture2D named tag in ResourceManager.
/// and return bound m_texture as a shape of pointer.
///
/// @param[in] texture_name The tag name of bound m_texture.
/// @return Raw pointer of m_texture.
///
TTextureRaw GetTexture(const std::string& texture_name);

///
/// @brief Destory m_texture and release memory space.
/// If class did not find bound m_texture with tag, do nothing and error flag set up.
///
/// @param[in] texture_name The tag name of bound m_texture.
///
void Release(const std::string& texture_name);

///
/// @brief Release all resource and clear.
///
void Clear();

///
/// @brief Check there is already Texture content with name.
///
bool IsTextureExist(const std::string& texture_name);

} /// ::opgs16::manager::texture namespace

#endif /// OPGS16_SYSTEM_MANAGER_TEXTURE_MANAGER_H