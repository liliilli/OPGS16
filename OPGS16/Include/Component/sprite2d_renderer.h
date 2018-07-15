#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/sprite2d_renderer.h
///
/// @brief The file contains sprite renderer class members.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-26 Componentization of Sprite2DRenderer.
/// 2018-02-28 Add Get/SetTextureIndex() member function.
/// 2018-03-07 Move file to /Component.
/// 2018-04-06 Abandon IndexSize structure indicates texture index, replace it with one unsigned value.
/// 2018-04-08 Supporting change of shader on running.
/// 2018-07-15 Remove pimpl.
///

#include <memory>

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h> /// Component macroes
/// ::opgs16::component::_internal::CRendererBase
#include <Component/Internal/renderer_base.h>
#include <Helper/Type/vectori2.h>

#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16::component {

enum class EPrimitiveType {
  Point,
  Line,
  LineLoop,
  Triangle,
};

///
/// @class CSprite2DRenderer
/// @brief The class for object has to render 2d rendering sprite.
/// SpriteRender class has sprite to render on object position,
/// and has at least one more shader to display to screen.
///
class CSprite2DRenderer final : public _internal::CRendererBase {
  using TPimplSmtPtr = std::unique_ptr<_internal::CSpriteRendererImpl>;

public:
	CSprite2DRenderer(element::CObject& bind_object,
                    const std::string& sprite_tag,
                    const std::string& shader_tag,
                    const int32_t texture_index = 0, const int32_t layer = 0);

  ///
  /// @brief Set new texture replacing present bound texture.
  /// @param[in] texture_name Texture tag name.
  ///
  void SetTexture(const std::string& texture_name);

  const std::string& GetTextureName() const noexcept;

  ///
  /// @brief Set texture index to display.
  /// @param[in] index_value Texture fragment value.
  ///
  void SetTextureFragmentIndex(int32_t index_value);

  int32_t GetTextureFragmentIndex() const noexcept;

	/*! Get ShaderWrapper instance. */
	element::CShaderWrapper& GetWrapper() noexcept;

  ///
  /// @brief Set Shader newly.
  /// @param[in] shader_name Shader name to specify.
  ///
  void SetShader(const std::string& shader_name);

  void SetInstanceCount(int32_t instance_count);

  int32_t GetInstanceCount() const noexcept;

  void SetPrimitiveMode(EPrimitiveType primitive_type);

  EPrimitiveType GetPrimitiveMode() const noexcept;

  ///
  /// @brief Render sprite on screen. Procedure is below.
  /// 1. m_shader is enable (must be enabled), active shader to use.
  /// 2. update shader uniform parameter.
  /// 3. bind texture, render it with final position(PVM) of bound object.
  ///
  void RenderSprite();

  ~CSprite2DRenderer();

private:
  EPrimitiveType m_primitive_type = EPrimitiveType::Triangle;
  GLenum  m_primitive_enum = GL_TRIANGLES;

  int32_t m_texture_fragment_index = 0;
  int32_t m_instance_count = 1;
  int32_t m_base_instance = 0;

  DVectorInt2 m_sprite_fragment_size;

  /// Sprite 2d texture stores image information.
	texture::CTexture2D* m_sprite = nullptr;
  /// Shader is in ShaderManager, render sprite.
  element::CShaderWrapper m_wrapper;
  /// Quad VAO to render sprite on screen.
  element::CVaoContainer* m_weak_vao_ref = nullptr;

  SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CSprite2DRenderer)
};

} /// ::opgs16::component namespace

#endif /** OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H */