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
private:
	struct ShaderParameters {
		std::unordered_map<std::string, GLfloat> m_floats{};
	};

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
		GLint internal_format,
		GLenum format,
		GLenum type,
		GLint width = 0, GLint height = 0);

	/** Initiate default depth buffer to [0] position of m_common_buffers. */
	[[noreturn]] void InitiateDefaultDepthBuffer();

	/**
	 * @brief Get reference of binded texture.
	 * @param[in] id Index to verify.
	 * @return The reference of std::unique_ptr<helper::Texture2D>.
	 */
	using texture_ptr = std::unique_ptr<texture::Texture2D>;
	texture_ptr& GetTexture(const size_t id) {
		return m_color_buffers.at(id);
	}

	/**
	 * @brief Bind texture to specific frame buffer with attributes.
	 * @param[in]
	 * @param[in]
	 */
	[[noreturn]] void BindTextureToFrameBuffer(const size_t texture_id,
		const size_t framebuffer_id,
		const GLenum attachment,
		const GLenum target);

	/**
	 * @brief
	 */
	[[noreturn]] void InitiateShader(const std::string& name, const std::string& pixel_shader);

	/**
	 * @brief Insert uniform variable value to be used by shader.
	 * @param[in] _Ty
	 * @param[in] value
	 */
	template <typename _Ty>
	[[noreturn]] void InsertUniformValue(const std::string tag, const _Ty value) {
		if (!IsValueAlreadyExist(tag, value)) {
			if (std::is_same_v<float, _Ty>) m_parameters.m_floats[tag] = value;
		}
	}

	template <typename _Ty>
	[[noreturn]] void ReplaceUniformValue(const std::string tag, const _Ty value) {
		if (IsValueAlreadyExist(tag, value))
			GetIteratorOfSpecifiedPoint(tag, value)->second = value;
	}

private:
	std::array<GLuint, 4> m_frame_buffers{};		/** */
	std::array<texture_ptr, 4> m_color_buffers{};	/** */
	std::array<GLuint, 8> m_common_buffers{};		/** */

	std::vector<std::shared_ptr<helper::ShaderNew>> m_shaders;
	ShaderParameters m_parameters{};	/** Uniform arguments container to use shader */

	GLuint empty_vao;
	bool m_is_useable{ false };

private:
	/**
	 * @brief Set uniform variables of shader with new values.
	 */
	[[noreturn]] void RefreshUniformValues(std::shared_ptr<helper::ShaderNew>& shader);

	/**
	 * @brief This method gets quad vertex attribute object.
	 * @return Lvalue reference of quad BindingObject shared with all pp frame instance.
	 */
	helper::BindingObject& GetCommonQuadVao();

	/**
	 * @brief This method checks wherther it already has a value on spot you want.
	 * @param[in] id Index to verify.
	 * @param[in] buffer Container to be verified by id.
	 * @return Success or failure flag. Return true if buffer in index is already generated.
	 * (except for helper::Texture2D. helper::Texture2D version is below.)
	 */
	template <size_t _Amnt>
	bool IsAlreadyGenerated(const size_t id, const std::array<GLuint, _Amnt>& buffer) const {
		if (id < _Amnt && buffer[id] == 0) return false; else return true;
	}

	/** Overriden method of IsAlreadyGenerated<size_t>(const size, const std::array). */
	bool IsAlreadyGenerated(const size_t id, const decltype(m_color_buffers)& buffer) const {
		if (id < buffer.size() && buffer[id] == nullptr) return false; else return true;
	}

	/**
	 * @brief This
	 * @param
	 * @return
	 */
	template <typename _Ty>
	auto GetIteratorOfSpecifiedPoint(const std::string& tag, const _Ty) {
		//-> decltype(std::unordered_map<std::string, _Ty>::iterator) {
		/** Body */
		if (std::is_same_v<float, _Ty>)
			return m_parameters.m_floats.find(tag);
	}

	/**
	 * @brief
	 * @param[in] tag The tag to find and check if it's exist.
	 * @param[in] _Ty Type paramter to check container type in m_paramaters of this.
	 * @return The flag accounts for success or failure of finding one.
	 */
	template <typename _Ty>
	bool IsValueAlreadyExist(const std::string& tag, const _Ty) {
		if (std::is_same_v<float, _Ty>)
			return m_parameters.m_floats.find(tag) != m_parameters.m_floats.end();
	}

};

}

#endif /** OPENGL_TUTORIALS_SYSTEM_SHADER_POST_PROCESSING_FRAME_H */
