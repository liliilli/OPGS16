#ifndef OPGS16_SYSTEM_SHADER_IMPL_POST_PROCESSING_FRAME_IMPL_H
#define OPGS16_SYSTEM_SHADER_IMPL_POST_PROCESSING_FRAME_IMPL_H

/*!
 * @file System\Shader\Impl\pp_frame_impl.h
 * @brief
 */

#include <array>        /*! std::array */
#include <memory>       /*! std::memory */
#include <unordered_map>/*! std::unordered_map */
#include <GL\glew.h>    /*! GLuint */

/// ::opgs16::texture::CTexture2DSprite
#include <Element/Internal/texture2d_sprite.h>
/// Forward declaration
#include <opgs16fwd.h>

namespace shading {

class PostProcessingImpl final {
public:

private:
    std::array<GLuint, 4> m_frame_buffers{};		/** Frame buffer container */
    using texture_ptr = std::unique_ptr<opgs16::texture::CTexture2DSprite>;
	std::array<texture_ptr, 4> m_color_buffers{};	/** Color buffer container */
	std::array<GLuint, 8> m_common_buffers{};		/** Universal buffer container */
	std::vector<opgs16::element::CShaderNew*> m_shaders;

	struct ShaderParameters {
		std::unordered_map<std::string, GLfloat> m_floats{};
	} m_parameters{};	/** Uniform arguments container to use shader */

	GLuint empty_vao;
	bool m_is_useable{ false };		/** Must be true to use post-processing instance */
	unsigned m_active_count{ 0 };

	enum class ErrorFlag {
		OK,
		/** Errors */
		SIZE_ARGUMENT_IS_NEGATIVE,
		COLOR_BUFFER_ALREADY_GENERATED,
		FRAME_BUFFER_ALREADY_GENERATED,
		NOT_FOUND_APPROPRIATE_FB_CB_ID,
		NOT_INITIATED_YET,
	} m_flag = ErrorFlag::OK;
};

}

#endif // !OPGS16_SYSTEM_SHADER_IMPL_POST_PROCESSING_FRAME_IMPL_H
