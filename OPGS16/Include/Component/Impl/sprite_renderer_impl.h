#ifndef OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
#define OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/sprite_renderer_impl.h
///
/// @brief Implementation file of CSprite2DRenderer
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-28 Change constructor to use texture_index type.
/// 2018-03-10 Refactoring.
/// 2018-04-08 Supporting change of shader on running.
/// 2018-06-13 Restructure to use vao items from management container.
///

/// CVertexArrayObject
#include <Element/Internal/vertex_array_object.h>

/// ::opgs16::element::_internal::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>
/// ::opgs16::element::CVaoContainer
#include <Element/Internal/vao_container.h>
/// ::opgs16::resource::STextrue2D::IndexSize
#include <Manager/resource_type.h>
/// ::opgs16::manager::MTextureManager
#include <Manager/texture_manager.h>
/// ::opgs16::shader::CShaderWrapper
#include <Shader/shader_wrapper.h>

/// forward declaration
#include <opgs16fwd.h>

namespace opgs16::component::_internal {

///
/// @class CSpriteRendererImpl
/// @brief Pointer to implementation structure.
///
/// @log
/// 2018-02-28
/// Change constructor to use texture_index type. and variable store texture index.
/// Add related boilerplate function.
/// 2018-04-02 std::string to std::wstring for Unicode
/// 2018-04-08 Supporting change of shader on running.
/// 2018-06-13 Restructure to use vao items from management container.
///
class CSpriteRendererImpl final {
public:
  CSpriteRendererImpl(const std::string& sprite_name,
                      const std::string& shader_name,
                      uint32_t texture_index);

  ~CSpriteRendererImpl();

  ///
  /// @brief
  /// Get shader wrapper;
  ///
  element::CShaderWrapper& Wrapper() {
    return m_wrapper;
  }

  ///
  /// @brief
  /// Get texture fragment index. if this does not handle atlas, return 0.
  ///
  uint32_t TextureIndex() const noexcept {
    return m_texture_fragment_index;
  }

  ///
  /// @brief Set texture fragment index with new_index value.
  /// @param[in] new_index
  ///
  void SetTextureIndex(const uint32_t new_index) noexcept;

  /*!*/
  void SetTexture(const std::string& texture_name) noexcept;

  /*!*/
  void SetShader(const std::string& shader_name);

  /*!*/
  void RenderSprite();

  /*!*/
  void SetInstanceCount(unsigned instance_count) {
    m_instance_count = instance_count;
  }

private:
  // Sprite 2d texture stores image information.
	texture::CTexture2D* m_sprite;
  // Shader is in ShaderManager, render sprite.
  element::CShaderWrapper m_wrapper;
  // Quad VAO to render sprite on screen.
  element::CVaoContainer* m_weak_vao_ref = nullptr;

  uint32_t  m_texture_fragment_index;
  GLenum    m_primitive_mode{ GL_TRIANGLES };
  uint32_t  m_instance_count{ 1 };
  uint32_t  m_base_instance{ 0 };
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
