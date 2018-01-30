#ifndef OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_FRAME_H
#define OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_FRAME_H

/**
 * @file System/Shader/pp_frame.h
 * @brief Elementary post-processing frame to manage things to render.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <array>	/** std::array */
#include <memory>	/** std::shared_ptr */
#include <vector>	/** std::vector<std::shared_ptr> */
#include "shader.h"	/** helper::ShaderNew */
#include "..\Frame\helper.h"
#include "..\Frame\texture.h"

/**
 * @namespace shading
 */
namespace shading {

/**
 * @class PostProcessingFrame
 * @brief The frame manages things to render on post-processing time.
 *
 * This class's objects has frame buffer, texture, and render buffer to be able to bind
 * frame buffer object.
 * And PostProcessingFrame has also shaders to render post-processing effects.
 *
 * Each frame is managed by PostProcessingManager, you can call and bind it through Manager.
 */
class PostProcessingFrame {
public:
	/**
	 * @brief
	 */
	[[noreturn]] void Initiate();

	/**
	 * @brief
	 */
	[[noreturn]] void Bind();

	/**
	 * @brief
	 */
	[[noreturn]] void RenderEffect();

	/**
	 * @brief Insert new frame buffer.
	 */
	[[noreturn]] void InsertFrameBuffer(const unsigned id);

	/**
	 * @brief Insert new color buffer.
	 */
	[[noreturn]] void InsertColorBuffer(const unsigned id,
		GLint internal_format, GLenum format, GLenum type, GLint width = 0, GLint height = 0);

private:
	std::array<GLuint, 4> m_frame_buffers{};
	using texture_ptr = std::unique_ptr<texture::Texture2D>;
	std::array<texture_ptr, 4> m_color_buffers{};
	std::array<GLuint, 8> m_common_buffers{};

	std::vector<std::shared_ptr<helper::ShaderNew>> m_shaders;

	GLuint empty_vao;
	bool m_is_useable{ false };
private:
	/**
	 * @brief
	 */
	[[noreturn]] void InitiateShader();

	/**
	 * @brief
	 */
	[[noreturn]] void InitiateDefaultDepthBuffer();

	/**
	 * @brief This method checks wherther it already has a value on spot you want.
	 */
	template <size_t _Amnt>
	bool IsAlreadyGenerated(const size_t i, const std::array<GLuint, _Amnt>& buffer) const {
		if (i < _Amnt && buffer[i] == 0) return false; else return true;
	}

	bool IsAlreadyGenerated(const size_t i, const decltype(m_color_buffers)& buffer) const {
		if (i < buffer.size() && buffer[i] == nullptr) return false; else return true;
	}

	/**
	 * @brief This method gets quad vertex attribute object.
	 */
	helper::BindingObject& GetCommonQuadVao();
};

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_FRAME_H */
