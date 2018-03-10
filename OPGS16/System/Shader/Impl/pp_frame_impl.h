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
#include "..\..\Frame\texture.h"            /*! texture::Texture2D */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! helper::ShaderNew */

namespace shading {

class PostProcessingImpl final {
public:

private:
    std::array<GLuint, 4> m_frame_buffers{};		/** Frame buffer container */
    using texture_ptr = std::unique_ptr<opgs16::texture::Texture2D>;
	std::array<texture_ptr, 4> m_color_buffers{};	/** Color buffer container */
	std::array<GLuint, 8> m_common_buffers{};		/** Universal buffer container */
	std::vector<helper::ShaderNew*> m_shaders;

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
