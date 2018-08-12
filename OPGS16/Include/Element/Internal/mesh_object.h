#ifndef OPGS16_ELEMENT_INTERNAL_MESH_OBJECT_H
#define OPGS16_ELEMENT_INTERNAL_MESH_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/mesh_object.h
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
/// The data structure have mesh information such as material meta data,
/// vertices like a local position, normal, tangent, and texutre coordination,
/// and indices with activated flags.
///
/// @log
/// 2018-06-13 Create file.
///
class DMeshObject final {
  using EActivated = phitos::enums::EActivated;

public:
  void VerticesReserve(uint32_t capacity) noexcept;

  ///
  /// @brief
  /// Push DMeshVector which has vector information into container.
  /// This funciton must be called after subject's m_is_vertices_activated is on.
  /// or hvae undefined behavior and malfunction.
  ///
  /// @param[in] vector Vector information.
  ///
  void PushVertice(const DMeshVector& vector) noexcept;

  ///
  /// @brief
  /// Push index for EBO into container.
  /// THis function must be called after m_is_indice _activated is on.
  /// Otherwise, function will cause undefined behavior or malfunction.
  ///
  /// @param[in] index Element index.
  ///
  void PushIndice(const uint32_t index) noexcept;

  ///
  /// @brief
  /// Push material meta information which has string and type and directory path
  /// into container. This function must be called after m_is_material_activated
  /// is on; Otherwise, function will cause undefined behavior or malfunction.
  ///
  /// @param[in] container Meta information instance.
  ///
  void PushMaterials(const std::vector<DMeshTextureMetaInfo>& container);

  ///
  /// @brief
  /// Activate m_is_vertice_activated once.
  /// Reverting function is not exist.
  ///
  void ActivateVertices() noexcept;

  ///
  /// @brief
  /// Activate m_is_indice_activated once.
  /// Reverting function is not exist.
  ///
  void ActivateIndices() noexcept;

  ///
  /// @brief
  /// Activate m_is_material_activated once.
  /// Reverting function is not exist.
  ///
  void ActivateMaterials() noexcept;

  ///
  /// @brief
  /// Activate this mesh object to create vao and vbo later time..
  /// Reverting function is not exist.
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

  ///
  /// @brief
  /// Check if vertices is activated on mesh object.
  ///
  inline EActivated IsVerticeActivated() const noexcept {
    return m_is_vertices_activated;
  }

  ///
  /// @brief
  /// Check if indices is activated on mesh object.
  ///
  inline EActivated IsIndiceActivated() const noexcept {
    return m_is_indice_activated;
  }

  ///
  /// @brief
  /// Check if material is activated on mesh object.
  ///
  inline EActivated IsMaterialActivated() const noexcept {
    return m_is_material_activated;
  }

  ///
  /// @brief
  /// Return byte size of vertices.
  ///
  inline uint32_t GetByteSizeOfVertices() const noexcept {
    return static_cast<uint32_t>(m_vertices.size()) *
           static_cast<uint32_t>(sizeof DMeshVector);
  }

  ///
  /// @brief
  /// Return byte size of indices.
  ///
  inline uint32_t GetByteSizeOfIndices() const noexcept {
    return static_cast<uint32_t>(m_element_indices.size()) *
           static_cast<uint32_t>(sizeof uint32_t);
  }

  ///
  /// @brief
  ///
  void* GetVerticesData() const noexcept {
    // ReSharper disable once CppCStyleCast
    return (void*)m_vertices.data();
  }

  ///
  /// @brief
  ///
  void* GetIndiceData() const noexcept {
    return (void*)m_element_indices.data();
  }

  ///
  /// @brief Get vertices size.
  ///
  int32_t GetVerticesSize() const noexcept {
    return static_cast<int32_t>(m_vertices.size());
  }

  ///
  /// @brief Get indices size.
  ///
  int32_t GetIndicesSize() const noexcept {
    return static_cast<int32_t>(m_element_indices.size());
  }

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