#ifndef OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
#define OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H

/*!
 * @file System\Object\Impl\sprite_renderer.h
 * @brief
 * @date 2018-02-08
 */

#include "..\..\Shader\shader_wrapper.h"        /*! ShaderWrapper */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! texture::Texture2D */
#include "..\..\Frame\vertex_array_object.h"    /*! VertexArrayObject */

/*!
 * @class
 * @brief
 */
class SpriteRendererImpl final {
public:
    /**
	 * @brief Make SpriteRenderer instance. (Constructor)
	 */
	SpriteRendererImpl(const std::string& image_path,
                       const std::string& shader_tag,
                       const unsigned layer = 0);

    [[noreturn]] inline void SetLayer(const unsigned layer) {

    }

    const unsigned GetLayer() const {
        return m_layer;
    }

    inline ShaderWrapper& GetWrapper() {
        return m_wrapper;
    }

    [[noreturn]] void RenderSprite();

private:
	texture::Texture2D* m_sprite;	/** Sprite 2d texture stores image information. */
	VertexArrayObject m_vao;	    /** Quad VAO to render sprite on screen. */
	ShaderWrapper m_wrapper;		/** Shader is in ShaderManager, render sprite. */
	unsigned m_layer;				/** Layer ordering number. The bigger, The later. */
};

#endif // !OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
