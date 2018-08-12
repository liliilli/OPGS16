#ifndef OPGS16_COMPONENT_MESH_RENDERER_H
#define OPGS16_COMPONENT_MESH_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/mesh_renderer.h
///
/// @brief
/// Mesh renderer.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file.
///

#include <Component/Internal/renderer_base.h>
#include <Component/mesh_importer.h>

namespace opgs16::component {

class CMeshRenderer final : public _internal::CRendererBase {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CMeshRenderer)
public:
  CMeshRenderer(element::CObject& bind_object, CMeshImporter& mesh_importer, const std::string& shader_name);
  CMeshRenderer(element::CObject& bind_object, CMeshImporter& mesh_importer);

	///
  /// @brief Get ShaderWrapper instance.
  ///
	element::CShaderWrapper& Getwrapper() noexcept;

  ///
  /// @brief Set Shader newly.
  /// @param[in] shader_name Shader name to specify.
  ///
  void SetShader(const std::string& shader_name);

  ///
  /// @brief Render sprite on screen. Procedure is below.
  /// 1. m_shader is enable (must be enabled), active shader to use.
  /// 2. update shader uniform parameter.
  /// 3. bind texture, render it with final position(PVM) of bound object.
  ///
  void Render();

private:
  CMeshImporter* m_mesh_importer = nullptr;
  element::CShaderWrapper m_wrapper;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_MESH_RENDERER_H