#include "pp_frame.h"

#include <iostream> /*! std::cerr
                      * std::endl */
#include "..\Frame\vertex_array_object.h"   /*! VertexArrayObject */
#include "shader_manager.h"

namespace shading {

constexpr std::array<unsigned, 6> quad_indices = { 0, 1, 2, 2, 3, 0 };
constexpr std::array<float, 32> quad_info = {
	// Vertex       //Normal        // TexCoord
	1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 1.f,
	1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 0.f,
	-1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 0.f,
	-1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 1.f
};

void PostProcessingFrame::Initiate() {
	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);
	m_is_useable = true;
}

void PostProcessingFrame::InsertFrameBuffer(const unsigned id) {
	if (!IsAlreadyGenerated(id, m_frame_buffers)) {
		glGenFramebuffers(1, &m_frame_buffers.at(id));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(id));
	}
	else { /** Set up error flag */
		m_flag = ErrorFlag::FRAME_BUFFER_ALREADY_GENERATED;
	}
}

void PostProcessingFrame::InsertColorBuffer(const unsigned id,
                                            GLint internal_format, GLenum format,
                                            GLenum type, GLint width,
                                            GLint height) {
	/*! Error checking */
	if (width < 0 || height < 0) { /** Set up error flag */
		m_flag = ErrorFlag::SIZE_ARGUMENT_IS_NEGATIVE; return;
	}

	if (IsAlreadyGenerated(id, m_color_buffers)) {
		m_flag = ErrorFlag::COLOR_BUFFER_ALREADY_GENERATED; return;
	}

	/*! Body */
    auto t_width = width;
    auto t_height = height;

    /*! Resize size components have zero value. */
    std::array<GLint, 4> screen_coord{ 0, 0, 256, 224 };
    if (t_width == 0 || t_height == 0) {
        //glGetIntegerv(GL_VIEWPORT, &screen_coord[0]);
        if (t_width == 0)   t_width = screen_coord[2];
        if (t_height == 0)  t_height = screen_coord[3];
    }

    /*! Insert. */
	m_color_buffers[id] = std::make_unique<texture::Texture2D>(internal_format,
                                                               format, type,
                                                               width, height);
}

void PostProcessingFrame::InitiateShader(const std::string& name) {
	/** Check If pp+Name is exist, push created shader */
    m_shader_wrapper.SetShader(ShaderManager::GetInstance().GetShaderWithName("pp" + name));
}

void PostProcessingFrame::InitiateDefaultDepthBuffer() {
	GLuint& depth_buffer = m_common_buffers[0];

	std::array<GLint, 4> size{};
	glGetIntegerv(GL_VIEWPORT, &size[0]);

	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size[2], size[3]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
}

void PostProcessingFrame::BindTextureToFrameBuffer(
	const size_t texture_id,
	const size_t framebuffer_id,
	const GLenum attachment,
	const GLenum target) {
	/** Body, Check if both texture and framebuffer are exist. */
	if (IsAlreadyGenerated(framebuffer_id, m_frame_buffers) &&
		IsAlreadyGenerated(texture_id, m_color_buffers)) {

		/** Bind */
		auto GL_FB = GL_FRAMEBUFFER;
		glBindFramebuffer(GL_FB, m_frame_buffers[framebuffer_id]);
		glFramebufferTexture2D(GL_FB, attachment, target, m_color_buffers[texture_id]->GetId(), 0);
		glBindFramebuffer(GL_FB, 0);
	}
	else { /** Error flag */
		m_flag = ErrorFlag::NOT_FOUND_APPROPRIATE_FB_CB_ID;
	}
}

VertexArrayObject& PostProcessingFrame::GetCommonQuadVao() {
    static VertexArrayObject quad_vao{ quad_info, 8, { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} }, quad_indices };
	return quad_vao;
}

void PostProcessingFrame::Bind() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else { m_flag = ErrorFlag::NOT_INITIATED_YET; }
}

void PostProcessingFrame::RenderEffect() {
	if (m_is_useable) {
		m_shader_wrapper.UseShader();
		glBindVertexArray(empty_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_buffers.at(0)->GetId());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
	else { m_flag = ErrorFlag::NOT_INITIATED_YET; }
}

/**
 * @brief Check if there is error.
 * If there is an error, output message in std::cerr.
 */
void PostProcessingFrame::CheckError() {
	switch (m_flag) {
	case ErrorFlag::SIZE_ARGUMENT_IS_NEGATIVE:
		std::cerr << "ERROR::PP_FRAME::SIZE_ARGUMENT_WAS_NEGATIVE" << std::endl; break;
	case ErrorFlag::COLOR_BUFFER_ALREADY_GENERATED:
		std::cerr << "ERROR::PP_FRAME::COLOR_BUFFER_ALREADY_GENERATED" << std::endl; break;
	case ErrorFlag::FRAME_BUFFER_ALREADY_GENERATED:
		std::cerr << "ERROR::PP_FRAME::FRAME_BUFFER_ALREADY_GENERATED" << std::endl; break;
	case ErrorFlag::NOT_FOUND_APPROPRIATE_FB_CB_ID:
		std::cerr << "ERROR::PP_FRAME::NOT_FOUND_APPROPRIATE_FB_CB_ID" << std::endl; break;
	case ErrorFlag::NOT_INITIATED_YET:
		std::cerr << "ERROR::PP_FRAME::NOT_INITIATED_YET" << std::endl; break;
	default: break;
	}
}

void PostProcessingFrame::Active() {
	m_active_count += 1;
}

void PostProcessingFrame::Disable() {
	if (IsActive()) m_active_count -= 1;
}

}
