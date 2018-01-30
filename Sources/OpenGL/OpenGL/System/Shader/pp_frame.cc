#include "pp_frame.h"
#include "shader_manager.h"

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

void shading::PostProcessingFrame::Initiate() {
	InsertFrameBuffer(0);
	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT, 720, 480);
	InitiateDefaultDepthBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	InitiateShader();

	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);

	m_is_useable = true;
}

void shading::PostProcessingFrame::InsertFrameBuffer(const unsigned id) {
	//if (!IsAlreadyGenerated(id, m_frame_buffers)) {
	if (!IsAlreadyGenerated(id, m_frame_buffers)) {
		glGenFramebuffers(1, &m_frame_buffers.at(id));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(id));
	}
	else { /** Set up error flag */

	}
}

void shading::PostProcessingFrame::InsertColorBuffer(const unsigned id,
	GLint internal_format, GLenum format, GLenum type, GLint width, GLint height) {
	/** Body */
	if (!IsAlreadyGenerated(id, m_color_buffers)) {
		auto temp =
			std::make_unique<texture::Texture2D>(internal_format, format, type, width, height);
		temp->SetTextureParameterI({
			{GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
			{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER}});
		temp->SetBorderColor({ 0, 0, 0, 1 });

		auto GL_FB = GL_FRAMEBUFFER;
		glFramebufferTexture2D(GL_FB, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, temp->GetId(), 0);

		m_color_buffers[id] = std::move(temp);
	}
	else { /** Set up error flag */

	}
}

void shading::PostProcessingFrame::InitiateShader() {
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

void shading::PostProcessingFrame::InitiateDefaultDepthBuffer() {
	GLuint& depth_buffer = m_common_buffers[0];

	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 720, 480);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
}

helper::BindingObject& shading::PostProcessingFrame::GetCommonQuadVao() {
	static helper::BindingObject quad_vao =
		helper::CreateBindingObjectEBO(quad_info, 8, { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} },
			quad_indices);
	return quad_vao;
}

void shading::PostProcessingFrame::Bind() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void shading::PostProcessingFrame::RenderEffect() {
	if (m_is_useable) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shaders.at(0)->Use();
		m_shaders.at(0)->SetFloat("uIntensity", 0.05f);
		glBindVertexArray(empty_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_buffers.at(0)->GetId());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}



