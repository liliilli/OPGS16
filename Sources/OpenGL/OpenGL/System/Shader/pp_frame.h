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

#include <memory>
#include <vector>
#include "shader.h"

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
	[[noreturn]] void Initiate();

	[[noreturn]] void Bind();

	[[noreturn]] void RenderEffect();

private:
	std::vector<GLuint> m_frame_buffers;
	std::vector<GLuint> m_color_buffers;
	std::vector<GLuint> m_common_buffers;

	std::vector<std::shared_ptr<helper::ShaderNew>> m_shaders;

	GLuint empty_vao;

	bool m_is_useable{ false };

private:
	[[noreturn]] void InitiateShader();
};

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_FRAME_H */
