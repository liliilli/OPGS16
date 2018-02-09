#include "sprite_renderer_impl.h"
#include "..\..\Frame\constant.h"   /*! std::array<> quad_info
                                      * std::array<> quad_indice */
#include "..\..\..\System\Manager\texture_manager.h"    /*! TextureManager */
#include "..\..\..\System\Shader\shader_manager.h"      /*! ShaderManager */

SpriteRendererImpl::SpriteRendererImpl(const std::string& sprite_tag,
                                       const std::string& shader_tag,
                                       const unsigned layer) :
    m_sprite{ TextureManager::GetInstance().GetTexture(sprite_tag) },
    m_vao{ quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices } {
    m_wrapper.SetShader(ShaderManager::GetInstance().GetShaderWithName(shader_tag));
}

void SpriteRendererImpl::RenderSprite() {
	m_wrapper.UseShader();
	glBindVertexArray(m_vao.GetVaoId());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sprite->GetId());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
