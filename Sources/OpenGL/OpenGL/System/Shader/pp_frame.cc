#include "pp_frame.h"
#include "shader_manager.h"

void shading::PostProcessingFrame::Initiate() {
	m_frame_buffers.resize(1);
	glGenFramebuffers(1, &m_frame_buffers.at(0));
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers.at(0));

	m_color_buffers.resize(1);
	glGenTextures(1, &m_color_buffers.at(0));
	glBindTexture(GL_TEXTURE_2D, m_color_buffers.at(0));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 720, 480, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	std::array<GLfloat, 4> border_color{ .0f, .0f, .0f, 1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, m_color_buffers.at(0), 0);

	m_common_buffers.resize(1);
	GLuint& depth_buffer = m_common_buffers[0];
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 720, 480);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	InitiateShader();

	/** Make empty vao for default_screen rendering */
	glGenVertexArrays(1, &empty_vao);

	m_is_useable = true;
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
		glBindTexture(GL_TEXTURE_2D, m_color_buffers.at(0));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}

