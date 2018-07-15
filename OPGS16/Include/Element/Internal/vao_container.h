#ifndef OPGS16_ELEMENT_INTERNAL_VAO_CONTAINER_H
#define OPGS16_ELEMENT_INTERNAL_VAO_CONTAINER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/vao_container.h
///
/// @brief
/// Internal vao container which has CInternalVertexArrayObject with model name.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-14 Create file.
///

#include <vector>

/// ::phitos::enums::EDirty
#include <Phitos/Enums/dirty.h>
/// ::opgs16::element::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>

//!
//! Forward declaration.
//!

namespace opgs16::component {
class CSprite2DRenderer;
class CProcedural2DRenderer;
}

//!
//! Implementation.
//!

namespace opgs16::element {

///
/// @enum EGcGeneration
///
/// @brief
/// CVaoContainer garbage collection.
///
enum class EGcGeneration {
  Zero,
  First,
  Second,

  DontRemove,
};

///
/// @class CVaoContainer
///
/// @brief
/// Vao container which stores dispensable internal vao objects.
///
/// @log
/// 2018-06-14 Create file.
///
class CVaoContainer final {
  using CInternalVertexArrayObject = _internal::CInternalVertexArrayObject;
  using TVaoList = std::vector<CInternalVertexArrayObject>;
public:
  ///
  /// @brief
  /// Make vao container which has internal vao instance per mesh in favor of
  /// model with model_name.
  ///
  /// @param[in] model_name Uniformed model and mesh name to save.
  /// @param[in] vao_list internal vao list to save.
  ///
  CVaoContainer(const std::string& model_name, const TVaoList& vao_list);

  ///
  /// @brief Return garbage collection generation age of this container.
  /// @return Garbage collection generation age.
  ///
  EGcGeneration GetGeneration() const noexcept {
    return m_generation;
  }

  ///
  /// @brief Get reference count from rendering component.
  /// @return reference count (but weak and fragile) from rendering component.
  ///
  /// @todo Replace it with
  ///
  uint32_t GetCount() const noexcept {
    return m_count;
  }

  ///
  /// @brief
  /// @return
  ///
  phitos::enums::EDirty IsDirty() const noexcept {
    return m_dirty;
  }

  ///
  /// @brief
  /// @return
  ///
  const TVaoList& GetVaoList() const noexcept {
    return m_vao_list;
  }

private:
  ///
  /// @brief Set dirty flag of vao container object.
  ///
  void SetDirty() noexcept {
    m_dirty = phitos::enums::EDirty::Dirty;
  }

  ///
  /// @brief Increase count internally.
  ///
  void IncreaseCount() noexcept {
    ++m_count;
  }

  ///
  /// @brief Decrease count internally.
  ///
  void DecreaseCount() noexcept {
    --m_count;
  }

  const TVaoList m_vao_list = {};
  const std::string m_container_name = "";

  uint32_t m_count = 0;
  phitos::enums::EDirty m_dirty = phitos::enums::EDirty::Clean;
  EGcGeneration m_generation = EGcGeneration::Zero;

  friend class component::CSprite2DRenderer;
  friend class component::CProcedural2DRenderer;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_INTERNAL_VAO_CONTAINER_H