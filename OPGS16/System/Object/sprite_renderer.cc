#include "sprite_renderer.h"

#include "..\Frame\constant.h"
#include "..\..\System\Manager\texture_manager.h"
#include "..\..\System\Shader\shader_manager.h"

SpriteRenderer::SpriteRenderer(const std::string& sprite_tag,
							   const std::string& shader_tag,
							   const unsigned layer) :
	m_sprite{ TextureManager::GetInstance().GetTexture(sprite_tag) },
	m_quad{ helper::CreateBindingObjectEBO(quad_info, 8,
										  {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}},
										  quad_indices) } {
	// Initiate Shader
	auto& manager = ShaderManager::GetInstance();
	m_wrapper.BindShader(manager.GetShaderWithName(std::string{ shader_tag }));
}

void SpriteRenderer::SetLayer(const unsigned layer) { m_layer = layer; }

const unsigned SpriteRenderer::GetLayer() const { return m_layer; }

void SpriteRenderer::RenderSprite() {
	m_wrapper.UseShader();
	glBindVertexArray(m_quad.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sprite->GetId());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
