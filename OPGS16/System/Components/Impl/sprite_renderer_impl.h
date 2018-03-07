#ifndef OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
#define OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H

/*!
 * @file System\Object\Impl\sprite_renderer.h
 * @brief

 * @author Jongmin Yun
 * @date 2018-02-28
 *
 * @log
 * 2018-02-28 Change constructor to use texture_index type.
 */

#include "..\..\Frame\vertex_array_object.h"    /*! VertexArrayObject */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! texture::Texture2D */
#include "../../Manager/Public/resource_type.h"        /*! resource::Texture2D::IndexSize */
#include "..\..\Shader\shader_wrapper.h"        /*! ShaderWrapper */

/*!
 * @class SpriteRendererImpl
 * @brief Pointer to implementation structure.
 *
 * @log
 * 2018-02-28 Change constructor to use texture_index type. and variable store texture index.
 * Add related boilerplate function.
 *
 * @todo Implement rendering layer priority feature.
 */
class SpriteRendererImpl final {
private:
    using IndexSize = opgs16::resource::Texture2D::IndexSize;

public:
    /** Make Sprite2DRenderer instance. (Constructor) */
	SpriteRendererImpl(const std::string& sprite_tag,
                       const std::string& shader_tag,
                       const opgs16::resource::Texture2D::IndexSize& texture_index,
                       unsigned layer);

    inline void SetLayer(const unsigned layer) {

    }

    inline unsigned GetLayer() const noexcept {
        return m_layer;
    }

    inline ShaderWrapper& GetWrapper() {
        return m_wrapper;
    }

    void RenderSprite();

    inline const IndexSize& GetTextureIndex() const noexcept {
        return m_index;
    }

    inline void SetTextureIndex(const IndexSize& new_index) noexcept {
        m_index = new_index;
        m_wrapper.ReplaceUniformValue("uTexIndex", glm::vec2{ m_index.x_sep, m_index.y_sep });
    }

private:
	texture::Texture2D* m_sprite;	/** Sprite 2d texture stores image information. */
	VertexArrayObject m_vao;	    /** Quad VAO to render sprite on screen. */
	ShaderWrapper m_wrapper;		/** Shader is in ShaderManager, render sprite. */
	unsigned m_layer;				/** Layer ordering number. The bigger, The later. */

    IndexSize m_index;

    GLuint empty_vao;
};

#endif // !OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
