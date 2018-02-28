#ifndef OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H
#define OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H

/**
 * @file System\Object\sprite_renderer.h
 * @brief The file contains sprite renderer class members.
 *
 * @author Jongmin Yun
 * @date 2018-02-28
 *
 * @log
 * 2018-02-26 Componentization of SpriteRenderer.
 * 2018-02-28 Add Get/SetTextureIndex() member function.
 */

#include <string>
#include <memory>
#include "..\..\Headers\Fwd\objectfwd.h"    /*! SpriteRendererImpl
                                              * SpriteRendererImplDeleter
                                              * ShaderWraper */
#include "..\Manager\resource_type.h"       /*! resource::Texture2D::IndexSize */

/**
 * @class SpriteRender
 * @brief The class for object has to render 2d rendering sprite.
 *
 * SpriteRender class has sprite to render on object position,
 * and has at least one more shader to display to screen.
 *
 * @date 2018-02-28
 *
 * @log
 * 2018-02-26 Componentization of SpriteRenderer.
 * 2018-02-28 Add Get/SetTextureIndex() member function.
 */
class SpriteRenderer final {
private:
    using IndexSize = resource::Texture2D::IndexSize;

public:
	/*! Make SpriteRenderer instance. (Constructor) */
	SpriteRenderer(const std::string& sprite_tag,
				   const std::string& shader_tag,
                   const resource::Texture2D::IndexSize& texture_index = {0, 0},
				   const unsigned layer = 0);

	/*!
	 * @brief Change layer number of this instance.
	 * @param[in] layer Layer number.
	 */
	void SetLayer(const unsigned layer);

	/*!
	 * @brief Get layer number of bound instance.
	 * @return layer number value.
	 */
	const unsigned GetLayer() const;

	/*! Get ShaderWrapper instance.  */
	ShaderWrapper& GetWrapper() const;

    const IndexSize& GetTextureIndex() const noexcept ;

    void SetTextureIndex(const IndexSize& new_index);

	/**
	 * @brief Render sprite on screen. Procedure is below.
	 * 1. m_shader is enable (must be enabled), active shader to use.
	 * 2. update shader uniform parameter.
	 * 3. bind texture, render it with final position(PVM) of bound object.
	 */
	 void RenderSprite();

private:
    using pimpl_type = std::unique_ptr<SpriteRendererImpl, SpriteRendererImplDeleter>;
    pimpl_type m_impl{ nullptr };
};

#endif /** OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H */