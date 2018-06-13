
///
/// @file Element/Internal/mesh_texture_meta.cc
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-13 Create file
///

/// Header file
#include <Element/Internal/mesh_texture_id.h>

#include <string>

namespace opgs16::element {

DMeshTextureMetaInfo::DMeshTextureMetaInfo(
    const std::string& texture_local_path,
    EMeshTextureType type) noexcept :
    m_texture_local_path(texture_local_path),
    m_type(type) {}

} /// ::opgs16::element namespace