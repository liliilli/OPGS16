#include <precompiled.h>
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

/// Header file
#include <Shader/framebuffer.h>

#include <Core/core_setting.h>
/// ::opgs16::element::CVertexArrayObject
#include <Element/Internal/vertex_array_object.h>
/// ::opgs16::element::_internal namespace
#include <Element/Internal/constant.h>
/// import logger
#include <Headers/import_logger.h>
/// ::opgs16::manager::ShaderManager
#include <Manager/shader_manager.h>
#include "Element/Internal/texture2d_plain.h"

namespace opgs16::element {

void CFrameBufferFrame::Initialize() {
	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);
	m_is_useable = true;
}

void CFrameBufferFrame::GenerateFrameBuffer(const unsigned id) {
  if (IsAlreadyGenerated(id, m_frame_buffers)) {
    PUSH_LOG_WARN("Already generated frame buffer.");
    return;
  }

  glGenFramebuffers(1, &m_frame_buffers[id]);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[id]);
}

void CFrameBufferFrame::GenerateColorBuffer(const unsigned id,
    GLint internal_format, GLenum format, GLenum type,
    GLint width, GLint height) {
  PHITOS_ASSERT(width > 0, "Width must not be less than 0.");
  PHITOS_ASSERT(height > 0, "Height must not be less than 0.");
  using opgs16::texture::CTexture2DSprite;

  // Error checking
  if (IsAlreadyGenerated(id, m_color_buffers)) {
    PUSH_LOG_ERRO("Failed to create color buffer. There is color buffer already.");
    return;
  }

  // Insert.
  auto texture = std::make_unique<texture::CTexture2DPlain>();
  texture->Initialize(internal_format, format, type, width, height);
  m_color_buffers[id] = std::move(texture);
}

void CFrameBufferFrame::GenerateDefaultColorBuffer() {
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;

  GenerateColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT, GetScreenWidth(), GetScreenHeight());
}

void CFrameBufferFrame::SetShader(const std::string& name) {
  m_shader_wrapper.SetShader(manager::shader::GetShader(name));
}

void CFrameBufferFrame::InitializeDefaultDepthBufferToFrameBuffer(int32_t framebuffer_index) {
  PHITOS_ASSERT(framebuffer_index >= 0, "Framebuffer index value must be positive or 0 integer.");
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;

  if (m_frame_buffers[framebuffer_index] == 0) {
    PHITOS_ASSERT(m_frame_buffers[framebuffer_index] != 0, "Framebuffer must be initialized prior to subsequent function call.");
    return;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[framebuffer_index]);

	GLuint& depth_buffer = m_common_buffers[0];
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GetScreenWidth(), GetScreenHeight());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferFrame::BindTextureToFrameBuffer(int32_t texture_id, int32_t framebuffer_id,
                                                 const GLenum attachment, const GLenum target) {
	/*! Check if both texture and framebuffer are exist. */
	if (IsAlreadyGenerated(framebuffer_id, m_frame_buffers) && IsAlreadyGenerated(texture_id, m_color_buffers)) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[framebuffer_id]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, m_color_buffers[texture_id]->GetTextureId(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
    PUSH_LOG_ERRO("Failed to bind texture to frame buffer.");
	}
}

void CFrameBufferFrame::Bind() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
    PUSH_LOG_ERRO("Failed to bind framebuffer, It's not initialized yet.");
	}
}

void CFrameBufferFrame::RenderEffect() {
	if (m_is_useable) {
		m_shader_wrapper.UseShader();
		glBindVertexArray(empty_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_buffers[0]->GetTextureId());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
	else {
    PUSH_LOG_ERRO("Failed to bind framebuffer, It's not initialized yet.");
	}
}

} /*! opgs16::element */
