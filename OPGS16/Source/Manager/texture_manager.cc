#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/texture_manager.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Refactoring.
/// 2018-07-29 Refactoring. Move class to namespace
///

/// Header file
#include <Manager/texture_manager.h>

#include <Element/Internal/texture2d_sprite.h>
#include <Manager/resource_manager.h>

namespace {
using opgs16::manager::texture::TTextureMap;

/// Texture container used in game.
TTextureMap m_container;
} /// ::unnamed namespace

namespace opgs16::manager::texture {

TTextureRaw GetTexture(const std::string& texture_name) {
  using opgs16::texture::CTexture2DSprite;
  if (IsTextureExist(texture_name))
    return m_container[texture_name].get();

  const auto container = resource::GetTexture2D(texture_name);
  if (!container) {
    PUSH_LOG_ERROR_EXT("Failed to find texture container, [Name : {}]", texture_name);
    return nullptr;
  }

  auto [result_it, result] = m_container.try_emplace(texture_name, nullptr);
  if (!result) {
    PUSH_LOG_ERROR_EXT("Failed to create texture resource, [Name : {}]", texture_name);
    return nullptr;
  }

  auto texture_obj = std::make_unique<CTexture2DSprite>();
  texture_obj->Initialize(*container);
  result_it->second.swap(texture_obj);
  return result_it->second.get();
}

void Release(const std::string& texture_name) {
	if (IsTextureExist(texture_name)) {
	  m_container.erase(texture_name);
  }
}

void Clear() {
  m_container.clear();
}

bool IsTextureExist(const std::string& texture_name) {
  return m_container.find(texture_name) != m_container.end();
}

} /// ::opgs16::manager::texture namespace
