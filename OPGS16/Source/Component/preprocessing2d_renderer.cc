
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Element/Private/preprocessing2d_renderer.cc
*
* @log
* 2018-04-29 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/preprocessing2d_renderer.h"             /*! Header file */
#include "../../Manager/Public/shader_manager.h"            /*! ::opgs16::manager::ShaderManager */
#include "../../Components/Public/preprocessing_getter.h"   /*! ::opgs16::component::CPreProcessingGetter */

namespace opgs16::component {

CPreProcessing2DRenderer::CPreProcessing2DRenderer(
    element::CObject& bind_object, component::CPreProcessingGetter* getter, 
    const std::string& shader_tag, const unsigned render_layer) :
    CRendererBase{ bind_object, render_layer }, m_getter{ getter } {
    /*! Body */
    m_wrapper.SetShader(manager::ShaderManager::Instance().Shader(shader_tag));
    glGenVertexArrays(1, &m_empty_vao);
}

void CPreProcessing2DRenderer::SetShader(const std::string & shader_name) {
    m_wrapper.SetShader(manager::ShaderManager::Instance().Shader(shader_name));
}

void CPreProcessing2DRenderer::Render() {
    m_wrapper.UseShader();  /*! the name is incorrect... */
    glBindVertexArray(m_empty_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_getter->GetOutputTextureId());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

} /*! opgs16::component */

#include "../Public/preprocessing2d_renderer.h"