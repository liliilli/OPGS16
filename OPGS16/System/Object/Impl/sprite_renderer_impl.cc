#include "sprite_renderer_impl.h"
#include "..\..\Frame\constant.h"   /*! std::array<> quad_info
                                      * std::array<> quad_indice */
#include "..\..\..\System\Manager\texture_manager.h"    /*! TextureManager */
#include "..\..\..\System\Shader\shader_manager.h"      /*! ShaderManager */

SpriteRendererImpl::SpriteRendererImpl(const std::string & sprite_tag,
                                       const std::string & shader_tag,
                                       const opgs16::resource::Texture2D::IndexSize& texture_index,
                                       unsigned layer) :
    m_sprite{ TextureManager::GetInstance().GetTexture(sprite_tag) },
    m_vao{ quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices },
    m_index { texture_index } {
    m_wrapper.SetShader(ShaderManager::GetInstance().GetShaderWithName(shader_tag));
    glGenVertexArrays(1, &empty_vao);

    auto [cell_x, cell_y] = m_sprite->GetTextureCellSize();
    m_wrapper.InsertUniformValue("uWHSize", glm::vec2{ cell_x, cell_y });
    m_wrapper.InsertUniformValue("uTexIndex",
                                 glm::vec2{
                                     (float)texture_index.x_sep,
                                     (float)texture_index.y_sep }
    );

    /*! Layer */
}

void SpriteRendererImpl::RenderSprite() {
	m_wrapper.UseShader();  /*! the name is incorrect... */
	//glBindVertexArray(m_vao.GetVaoId());
	glBindVertexArray(empty_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sprite->GetId());
    glDrawArrays(GL_TRIANGLES, 0, 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
