#ifndef OPGS16_ELEMENT_INTERNAL_MESH_TEXTURE_ID_H
#define OPGS16_ELEMENT_INTERNAL_MESH_TEXTURE_ID_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/mesh_texture_id.h
///
/// @brief
/// Internal Mesh material texture id structure.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-11 Create file.
///

#include <cstdint>

namespace opgs16::element {

///
/// @enum EMeshTextureType
///
/// @brief
/// Meta texture map meta information which specifies texture map type.
///
enum class EMeshTextureType {
  None,
  DiffuseMap,   /// "texture_diffuse"
  SpecularMap,  /// "texture_specular"
};

///
/// @class DMeshTextureMetaInfo
///
/// @brief
///
///
class DMeshTextureMetaInfo final {
public:
  DMeshTextureMetaInfo(const std::string& texture_local_path) noexcept;

private:
  std::string m_texture_local_path = "";
  EMeshTextureType m_type = EMeshTextureType::None;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_INTERNAL_MESH_TEXTURE_ID_H