#ifndef OPGS16_ELEMENT_INTERNAL_MESH_OBJECT_H
#define OPGS16_ELEMENT_INTERNAL_MESH_OBJECT_H

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

#include <vector>

#include <Phitos/Enums/activated.h>
#include <Phitos/Enums/initiated.h>

/// ::opgs16::element::DMeshVector
#include <Element/Internal/mesh_vector.h>
/// ::opgs16::element::DMeshTextureMetaInfo
#include <Element/Internal/mesh_texture_id.h>

namespace opgs16::element {

///
/// @class DMeshObject
///
/// @brief
///
///
class DMeshObject final {
  using EActivated = phitos::enums::EActivated;

public:
  void VerticesReserve(uint32_t capacity) noexcept;

  ///
  /// @brief
  ///
  /// @param[in] vector
  ///
  void PushVertice(const DMeshVector& vector) noexcept;

  ///
  /// @brief
  ///
  /// @param[in] index
  ///
  void PushIndice(const uint32_t index) noexcept;

  ///
  /// @brief
  ///
  /// @param[in] container
  ///
  void PushMaterials(const std::vector<DMeshTextureMetaInfo>& container);

  ///
  /// @brief
  ///
  void ActivateVertices() noexcept;

  ///
  /// @brief
  ///
  void ActivateIndices() noexcept;

  ///
  /// @brief
  ///
  void ActivateMaterials() noexcept;

  ///
  /// @brief
  ///
  void ActivateMeshObject() noexcept;

  ///
  /// @brief
  /// Activate position vertices flag,
  /// but m_is_vertices_activated must be activated prior to calling this function.
  ///
  void ActivatePosition() noexcept;

  ///
  /// @brief
  /// Activate normal vertices flag,
  /// but m_is_vertices_activated must be activated prior to calling this function.
  ///
  void ActivateNormal() noexcept;

  ///
  /// @brief
  /// Activate tangent and bitangent vertices flag,
  /// but m_is_vertices_activated must be activated prior to calling this function.
  ///
  void ActivateTangent() noexcept;

  ///
  /// @brief
  /// Activate texture coordinate vertices flag,
  /// but m_is_vertices_activated must be activated prior to calling this function.
  ///
  void ActivateTextureCoords() noexcept;

private:
  std::vector<DMeshVector>    m_vertices;
  std::vector<uint32_t>       m_element_indices;
  std::vector<DMeshTextureMetaInfo> m_materials;

  mutable EActivated m_is_vertices_activated = EActivated::Disabled;
  mutable EActivated m_is_indice_activated = EActivated::Disabled;
  mutable EActivated m_is_material_activated = EActivated::Disabled;

  mutable EActivated m_is_position_activated  = EActivated::Disabled;
  mutable EActivated m_is_normal_activated    = EActivated::Disabled;
  mutable EActivated m_is_tangent_activated   = EActivated::Disabled;
  mutable EActivated m_is_texcoord_activated  = EActivated::Disabled;

  using EInitiated = phitos::enums::EInitiated;
  mutable EInitiated m_is_object_initiated = EInitiated::NotInitiated;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_INTERNAL_MESH_OBJECT_H