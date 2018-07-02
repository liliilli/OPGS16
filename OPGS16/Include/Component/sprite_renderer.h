#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/sprite_renderer.h
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
///

#include <string>
#include <memory>

/// ::opgs16::component::_internal::CComponent
#include <Component\Internal\component.h>
#include <Component\Internal\component_macro.h> /// Component macroes
/// ::opgs16::component::_internal::CRendererBase
#include <Component\Internal\renderer_base.h>

#include <Manager\resource_type.h>
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16::component {

///
/// @class CSprite2DRenderer
/// @brief The class for object has to render 2d rendering sprite.
/// SpriteRender class has sprite to render on object position,
/// and has at least one more shader to display to screen.
///
/// @log
/// 2018-02-26 Componentization of Sprite2DRenderer.
/// 2018-02-28 Add Get/SetTextureIndex() member function.
/// 2018-03-07 Move to opgs16::component namespace.
/// 2018-04-06 Abandon IndexSize structure indicates texture index, replace it with one unsigned value.
/// 2018-04-08 Supporting change of shader on running.
///
class CSprite2DRenderer final : public _internal::CRendererBase {
private:
  using pimpl_type = std::unique_ptr<_internal::CSpriteRendererImpl>;

public:
	/*! Make Sprite2DRenderer instance. (Constructor) */
	CSprite2DRenderer(element::CObject& bind_object,
                    const std::string& sprite_tag,
                    const std::string& shader_tag,
                    const unsigned texture_index = 0,
                    const unsigned render_layer = 0);

  /*!
   * @brief Set new texture replacing present bound texture.
   * @param[in] texture_name Texture tag name.
   */
  void SetTexture(const std::string& texture_name);

  /*!
   * @brief Set texture index to display.
   * @param[in] index_value Texture fragment value.
   */
  void SetTextureIndex(const unsigned index_value);

  /*! Get Texture index position. */
  const unsigned TextureIndex() const noexcept;

	/*! Get ShaderWrapper instance. */
	element::CShaderWrapper& Wrapper() const;

  /*!
   * @brief Set Shader newly.
   * @param[in] shader_name Shader name to specify.
   */
  void SetShader(const std::string& shader_name);

  /*!*/
  void SetInstanceCount(unsigned instance_count);

  /**
   * @brief Render sprite on screen. Procedure is below.
   * 1. m_shader is enable (must be enabled), active shader to use.
   * 2. update shader uniform parameter.
   * 3. bind texture, render it with final position(PVM) of bound object.
   */
  void RenderSprite();

  ~CSprite2DRenderer();

private:
  pimpl_type m_impl{};

  SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CSprite2DRenderer)
};

} /// ::opgs16::component namespace

#endif /** OPGS16_SYSTEM_COMPONENTS_PUBLIC_SPRITE_RENDERER_H */