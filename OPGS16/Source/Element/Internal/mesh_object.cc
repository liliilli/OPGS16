
///
/// @file Element/Internal/model_object.cc
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-12 Create file
///

/// Header file
#include <Element/Internal/mesh_object.h>
/// Expanded assertion
#include <Phitos/Dbg/assert.h>
/// logger for debug mode
#include <Headers/import_logger.h>

namespace opgs16::element {

void DMeshObject::VerticesReserve(uint32_t capacity) noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");

  m_vertices.reserve(capacity);
}

void DMeshObject::PushVertice(const DMeshVector& vector) noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_vertices_activated == EActivated::Activated,
      "MeshObject vertices must be activated.");
}

void DMeshObject::PushIndice(const uint32_t index) noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_indice_activated == EActivated::Activated,
      "MeshObject indice must be activated.");

  m_element_indices.push_back(index);
}

void DMeshObject::PushMaterials(
    const std::vector<DMeshTextureMetaInfo>& container) {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_material_activated == EActivated::Activated,
      "MeshObject materials must be activated.");

  // It's might be buffer overflow.
  m_materials.insert(m_materials.end(), container.cbegin(), container.cend());
}

void DMeshObject::ActivateVertices() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_vertices_activated == EActivated::Disabled,
      "MeshObject vertices is already activated.");

  m_is_vertices_activated = EActivated::Activated;
}

void DMeshObject::ActivateIndices() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_indice_activated == EActivated::Disabled,
      "MeshObject indices are already activated.");

  m_is_indice_activated = EActivated::Activated;
}

void DMeshObject::ActivateMaterials() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_material_activated == EActivated::Disabled,
      "MeshObject materials are already activated.");

  m_is_material_activated = EActivated::Activated;
}

void DMeshObject::ActivateMeshObject() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "Duplicated activating mesh object is prohibited.");

  m_is_object_initiated = EInitiated::Initiated;
}

void DMeshObject::ActivatePosition() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_position_activated == EActivated::Disabled,
      "MeshObject materials are already activated.");

  m_is_position_activated = EActivated::Activated;
}

void DMeshObject::ActivateNormal() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_normal_activated == EActivated::Disabled,
      "MeshObject materials are already activated.");

  m_is_normal_activated = EActivated::Activated;
}

void DMeshObject::ActivateTangent() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_tangent_activated == EActivated::Disabled,
      "MeshObject materials are already activated.");

  m_is_tangent_activated = EActivated::Activated;
}

void DMeshObject::ActivateTextureCoords() noexcept {
  PHITOS_ASSERT(m_is_object_initiated == EInitiated::NotInitiated,
      "MeshObject is already initiated.");
  PHITOS_ASSERT(m_is_texcoord_activated == EActivated::Disabled,
      "MeshObject materials are already activated.");

  m_is_texcoord_activated = EActivated::Activated;
}

} /// ::opgs16::element namespace