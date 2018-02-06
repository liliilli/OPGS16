#ifndef OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H
#define OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H

/**
 * @file System\Object\sprite_renderer.h
 * @brief The file contains sprite renderer class members.
 * Last Updated 2018 - 02 - 04
 *
 *
 *
 * @author Jongmin Yun
 */

#include "..\Shader\shader_wrapper.h"
#include "..\..\Headers\common.h"

/**
 * @class SpriteRender
 * @brief The class for object has to render 2d rendering sprite.
 *
 * SpriteRender class has sprite to render on object position,
 * and has at least one more shader to display to screen.
 *
 * SpriteRender has layer, is sorted by internal mechanism and displayed in order.
 * The number of layers is restricted to 8 layer, 9 is UI, 10 is Global UI.
 *
 * Last Updated 2018 - 02 - 04
 */
class SpriteRenderer final {
public:
	/**
	 * @brief Make SpriteRenderer instance. (Constructor)
	 */
	SpriteRenderer(const std::string& image_path,
				   const std::string& shader_tag,
				   const unsigned layer = 0);

	/**
	 * @brief Change layer number of this instance.
	 * @param[in] layer Layer number.
	 */
	[[noreturn]] void SetLayer(const unsigned layer);

	/**
	 * @brief Get layer number of bound instance.
	 * @return layer number value.
	 */
	const unsigned GetLayer() const;

	/** * @brief Get Shaderwrapper instance.  */
	inline ShaderWrapper& GetWrapper();

	/**
	 * @brief Render sprite on screen. Procedure is below.
	 * 1. m_shader is enable (must be enabled), active shader to use.
	 * 2. update shader uniform parameter.
	 * 3. bind texture, render it with final position(PVM) of bound object.
	 */
	[[noreturn]] void RenderSprite();

private:
	texture::Texture2D* m_sprite;	/** Sprite 2d texture stores image information. */
	helper::BindingObject m_quad;	/** Quad VAO to render sprite on screen. */
	ShaderWrapper m_wrapper;		/** Shader is in ShaderManager, render sprite. */
	unsigned m_layer;				/** Layer ordering number. The bigger, The later. */
};

inline ShaderWrapper& SpriteRenderer::GetWrapper() {
	return m_wrapper;
}

#endif /** OPGS16_SYSTEM_OBJECT_SPRITE_RENDERER_H */