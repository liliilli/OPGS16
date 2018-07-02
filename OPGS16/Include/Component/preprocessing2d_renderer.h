#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_2D_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_2D_RENDERER_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file Component\preprocessing2d_renderer.h
*
* @log
* 2018-04-29 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// ::opgs16::component::_internal::CRendererBase
#include <Component\Internal\renderer_base.h>
#include <opgs16fwd.h> /// Forward declaration

namespace opgs16::component {

class CPreProcessing2DRenderer final : public _internal::CRendererBase {
public:
    CPreProcessing2DRenderer(element::CObject& bind_object, component::CPreProcessingGetter* getter,
                             const std::string& shader_tag, const unsigned render_layer = 0);

  ~CPreProcessing2DRenderer();

    element::CShaderWrapper& GetWrapper() {
        return m_wrapper;
    };

    /*!
     * @brief Set Shader newly.
     * @param[in] shader_name Shader name to specify.
     */
    void SetShader(const std::string& shader_name);

    void Render();

private:
    component::CPreProcessingGetter*    m_getter;
    element::CShaderWrapper             m_wrapper;
    unsigned m_empty_vao{ 0u };

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CPreProcessing2DRenderer)
};

} /*! ::opgs16::component */

#endif // OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_2D_RENDERER_H