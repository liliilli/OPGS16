#include "pp_frame.h"
#include "shader_manager.h"

namespace shading {

constexpr std::array<float, 32> quad_info = {
	// Vertex       //Normal        // TexCoord
	1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 1.f,
	1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 0.f,
	-1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 0.f,
	-1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 1.f
};

constexpr std::array<unsigned, 6> quad_indices = {
	// Primitive is triangle
	0, 1, 2,
	2, 3, 0
};

/** Methods body */

void PostProcessingFrame::Initiate() {
	InitiateShader();

	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);
	m_is_useable = true;
}

void PostProcessingFrame::InsertFrameBuffer(const unsigned id) {
	//if (!IsAlreadyGenerated(id, m_frame_buffers)) {
	if (!IsAlreadyGenerated(id, m_frame_buffers)) {
		glGenFramebuffers(1, &m_frame_buffers.at(id));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(id));
	}
	else { /** Set up error flag */

	}
}

void PostProcessingFrame::InsertColorBuffer(const unsigned id,
	GLint internal_format, GLenum format, GLenum type, GLint width, GLint height) {
	/** Body */
	if (!IsAlreadyGenerated(id, m_color_buffers)) {
		auto temp = std::make_unique<texture::Texture2D>(internal_format, format, type, width, height);
		m_color_buffers[id] = std::move(temp);
	}
	else { /** Set up error flag */

	}
}

void PostProcessingFrame::InitiateShader() {
	/** Make shader for temporary frame buffer */
	auto& manager = ShaderManager::GetInstance();

	auto shader = manager.GetShaderWithName("gConvex");
	if (!shader) {
		using Type = helper::ShaderNew::Type;
		using namespace std::string_literals;

		shader = manager.CreateShader("gConvex", {
			{ Type::VS, "Shaders/Global/quad.vert"s },
			{ Type::FS, "Shaders/Global/convex.frag"s }
			});
	}

	m_shaders.push_back(shader);
}

void PostProcessingFrame::InitiateDefaultDepthBuffer() {
	GLuint& depth_buffer = m_common_buffers[0];

	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 720, 480);
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
}

helper::BindingObject& PostProcessingFrame::GetCommonQuadVao() {
	static helper::BindingObject quad_vao =
		helper::CreateBindingObjectEBO(quad_info, 8, { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} },
			quad_indices);
	return quad_vao;
}

void PostProcessingFrame::Bind() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void PostProcessingFrame::RenderEffect() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shaders.at(0)->Use();
		RefreshUniformValues(m_shaders.at(0));
		glBindVertexArray(empty_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_buffers.at(0)->GetId());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}

void PostProcessingFrame::RefreshUniformValues(std::shared_ptr<helper::ShaderNew>& shader) {
	for (const auto& item : m_parameters.m_floats) {
		shader->SetFloat(item.first, item.second);
	}
}


}
