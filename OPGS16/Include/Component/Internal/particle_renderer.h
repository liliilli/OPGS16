#ifndef OPGS16_COMPONENT_INTERNAL_PARTICLE_RENDERER_H
#define OPGS16_COMPONENT_INTERNAL_PARTICLE_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-05 Create file.
///

#include <Component/Internal/renderer_base.h>

namespace opgs16::component::_internal {

class CInternalParticleRenderer final : public CRendererBase {
public:
  CInternalParticleRenderer(element::CObject& bind_object);

  ///
  /// @brief Set new texture replacing present bound texture.
  /// @param[in] texture_name Texture tag name.
  ///
  void SetTexture(const std::string& texture_name);

  ///
  /// @brief Set texture index to display.
  /// @param[in] index_value Texture fragment value.
  ///
  void SetTextureFragmentIndex(int32_t index_value);

  ///
  /// @brief
  ///
  void Render();

  ///
  /// @brief
  ///
  element::CShaderWrapper& GetShaderWrapper() noexcept;

private:
  ///
  ///
  ///
  void pResetTextureFragmentProperties();

  texture::CTexture2DSprite*    m_sprite       = nullptr;
  element::CVaoContainer* m_weak_vao_ref = nullptr;

  element::CShaderWrapper m_wrapper;
  int32_t m_texture_fragment_index = 0;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CInternalParticleRenderer)
};

} /// ::opgs16::component::_internal

#endif /// OPGS16_COMPONENT_INTERNAL_PARTICLE_RENDERER_H