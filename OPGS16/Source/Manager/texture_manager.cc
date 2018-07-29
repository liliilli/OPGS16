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

/// ::opgs16::texture::CTexture2D
#include <Frame/texture.h>
/// ::opgs16::manager::MResourceManager
#include <Manager/resource_manager.h>

namespace {
using opgs16::manager::texture::TTextureMap;

/// Texture container used in game.
TTextureMap m_container;
} /// ::unnamed namespace

namespace opgs16::manager::texture {

TTextureRaw GetTexture(const std::string& texture_name) {
  using opgs16::texture::CTexture2D;

  if (IsTextureExist(texture_name)) {
    return m_container[texture_name].get();
  }

  const auto container = manager::resource::GetTexture2D(texture_name);

  auto [it, good] = m_container.emplace(
      texture_name,
      std::make_unique<CTexture2D>(*container)
  );

  if (good) {
    return it->second.get();
  }

  return nullptr;
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
