
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/pp_frame.h
 * @brief Elementary post-processing frame to manage a thing to be rendered.
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Add boilerplate comments.
 * 2018-04-20 Remove error flags and add log output.
 * 2018-04-20 Moved namespace to ::opgs16::element and remove ::shading unknown malicious namespace.
 * 2018-04-21 Rename CPostProcessingFrame to CFrameBufferFrame.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Shader\framebuffer.h> /// Header file
/// ::opgs16::element::CVertexArrayObject
#include <Element/Internal/vertex_array_object.h>
/// ::opgs16::manager::ShaderManager
#include <Manager\shader_manager.h>
/// ::opgs16::element::_internal namespace
#include <Element\Internal\constant.h>
/// import logger
#include <Headers\import_logger.h>

namespace opgs16::element {

void CFrameBuferFrame::Initialize() {
	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);
	m_is_useable = true;
}

void CFrameBuferFrame::GenerateFrameBuffer(const unsigned id) {
    if (IsAlreadyGenerated(id, m_frame_buffers)) {
        PUSH_LOG_WARN("Already generated frame buffer.");
        return;
    }

    glGenFramebuffers(1, &m_frame_buffers[id]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[id]);
}

void CFrameBuferFrame::GenerateColorBuffer(const unsigned id, GLint internal_format, GLenum format,
                                           GLenum type, GLint width, GLint height) {
    /*! Error checking */
	if (IsAlreadyGenerated(id, m_color_buffers)) {
        PUSH_LOG_ERRO("Failed to create color buffer. There is color buffer already.");
	    return;
	}
	if (width < 0 || height < 0) {
        PUSH_LOG_ERRO("Failed to create color buffer. Either width or height is less than 0.");
	    return;
	}

    /*! Resize size components have zero value. */
    int t_width = width, t_height = height;
    if (t_width == 0 || t_height == 0) {
        if (t_width == 0)   t_width = _internal::screen_coord[2];
        if (t_height == 0)  t_height = _internal::screen_coord[3];
    }

    /*! Insert. */
    using opgs16::texture::CTexture2D;
	m_color_buffers[id] = std::make_unique<CTexture2D>(internal_format, format, type, t_width, t_height);
}

void CFrameBuferFrame::SetShader(const char* name) {
	/** Check If pp+Name is exist, push created shader */
    m_shader_wrapper.SetShader(manager::shader::GetShader(name));
}

void CFrameBuferFrame::InitializeDefaultDepthBuffer() {
	GLuint& depth_buffer = m_common_buffers[0];

	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _internal::screen_coord[2], _internal::screen_coord[3]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
}

void CFrameBuferFrame::BindTextureToFrameBuffer(
    int32_t texture_id, int32_t framebuffer_id,
    const GLenum attachment, const GLenum target) {
	/*! Check if both texture and framebuffer are exist. */
	if (IsAlreadyGenerated(framebuffer_id, m_frame_buffers) && IsAlreadyGenerated(texture_id, m_color_buffers)) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[framebuffer_id]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, m_color_buffers[texture_id]->Id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
        PUSH_LOG_ERRO("Failed to bind texture to frame buffer.");
	}
}

void CFrameBuferFrame::Bind() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
        PUSH_LOG_ERRO("Failed to bind framebuffer, It's not initialized yet.");
	}
}

void CFrameBuferFrame::RenderEffect() {
	if (m_is_useable) {
		m_shader_wrapper.UseShader();
		glBindVertexArray(empty_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_buffers[0]->Id());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
	else {
        PUSH_LOG_ERRO("Failed to bind framebuffer, It's not initialized yet.");
	}
}

CVertexArrayObject& CFrameBuferFrame::GetCommonQuadVao() {
    static CVertexArrayObject quad_vao{ _internal::quad_info, 8, { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} }, _internal::quad_indices };
	return quad_vao;
}

} /*! opgs16::element */
