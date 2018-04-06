#ifndef OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H
#define OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H

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
#include "shader.h"	/** helper::CShaderNew */
#include "..\Frame\texture.h"   /*! texture::CTexture2D */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! VertexArrayObject */
#include "shader_wrapper.h" /*! ShaderWrapper */

/**
 * @namespace shading
 * @brief shading namespace is for m_object_list related to shading and shader management.
 */
namespace shading {

/**
 * @class PostProcessingFrame
 * @brief The frame manages things to render on post-processing time.
 *
 * This class's m_object_list has frame buffer, texture, and render buffer to be able to bind
 * frame buffer object.
 * And PostProcessingFrame has also shaders to render post-processing effects.
 *
 * Each frame is managed by PostProcessingManager, you can call and bind it through Manager.
 * But you should not try to bind this itself, not Calling PostProcessingManager's BindSequence().
 * This will crash application or at least it incurs undefined behavior.
 */
class PostProcessingFrame {
public:
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	 virtual void Initiate();

	/**
	 * @brief Insert new frame buffer with id value.
	 * If there is already generated frame buffer in id index of m_frame_buffer,
	 * Set flag(ERR_DUPLICATED_FB) up on error variable. Error message can see through calling
	 * CheckError() method.
	 *
	 * @param[in] id The index to create it on container.
	 */
	 void InsertFrameBuffer(const unsigned id);

	/**
	 * @brief Insert new color buffer, with arguments need to be passed to making color buffer.
	 * @param[in] internal_format Color buffer's internal format how to specify a way of storing
	 * values in each pixels. Use OpenGL's format. (from table 1, 2, 3 below)
	 *
	 * @param[in] format Specifies the format of the pixel data.
	 * The following symbolic values are accepted.
	 * GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER,
	 * GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX,
	 * GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
	 *
	 * @param[in] type Specifies the data type of the pixel data.
	 * Use OpenGL's enum type written in below @see header. (type section)
	 *
	 * @param[in] width Buffer width. if this is blank, automatically binds it to resolution width.
	 * @param[in] height Buffer height. if this is blank, bind it to resolution height.
	 *
	 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
	 */
	void InsertColorBuffer(const unsigned id,
		GLint internal_format,
		GLenum format,
		GLenum type,
		GLint width = 0, GLint height = 0);

	/** Initiate default depth buffer to [0] position of m_common_buffers. */
	 void InitiateDefaultDepthBuffer();

	/**
	 * @brief Get reference of binded texture.
	 * @param[in] id Index to verify.
	 * @return The reference of std::unique_ptr<helper::CTexture2D>.
	 */
	using texture_ptr = std::unique_ptr<opgs16::texture::CTexture2D>;
	texture_ptr& GetTexture(const size_t id) { return m_color_buffers.at(id); }

	/**
	 * @brief Bind texture to specific frame buffer with attributes.
	 * If both texture and frame buffer id are not exist, this method does nothing.
	 *
	 * @param[in] texture_id Texture id to bind to frame buffer.
	 * @param[in] framebuffer_id Frame buffer id to bind.
	 * @param[in] attachment Texture's attachment attribute for binding to frame buffer.
	 * @param[in] target Target
	 *
	 * @see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
	 */
	 void BindTextureToFrameBuffer(const size_t texture_id,
                                               const size_t framebuffer_id,
                                               const GLenum attachment,
                                               const GLenum target);

	/**
	 * @brief Create and Initiate shader to be used only in this post-processing frame.
	 * If there is already existed shader, this methods does nothing.
	 *
	 * @param[in] name The name of shader to create.
	 * @param[in] pixel_shader The path of pixel shader.
	 */
	 void InitiateShader(const std::string& name);

	/**
	 * @brief Bind frame buffer. This must be called to render m_object_list to frame buffer.
	 * This methods could be overriden by derived class.
	 */
	 virtual void Bind();

	/**
	 * @brief Update frame buffer.
	 * This must be called to active post-processing effects.
	 * This methods could be overriden by derived class.
	 */
	 virtual void Update() {};

	/**
	 * @brief Render texture and components.
	 * This must be called after arbitary frame buffer bound.
	 * This methods could be overriden by derived class.
     *
     * Caller calls this method, Shader would be used and refresh uniform parameters automatically.
     * and texture binds, render.
	 */
	 virtual void RenderEffect();

	/**
	 * @brief Insert uniform variable value to be used by shader.
	 * @param[in] tag The tag to insert value.
	 * @param[in] _Ty Type parameter to compare with supported uniform container's type.
	 * @param[in] value The value insert.
	 */
	template <typename _Ty>
	 void InsertUniformValue(const std::string tag, const _Ty value) {
		if (!IsValueAlreadyExist(tag, value)) m_shader_wrapper.SetUniformValue(tag, value);
	}

	/**
	 * @brief Replace uniform variable's value with new value.
	 * @param[in] tag The tag to insert value.
	 * @param[in] _Ty Type parameter to compare with supported uniform container's type.
	 * @param[in] value The value insert.
	 */
	template <typename _Ty>
	 void ReplaceUniformValue(const std::string tag, const _Ty value) {
        if (IsValueAlreadyExist(tag, value)) m_shader_wrapper.SetUniformValue(tag, value);
	}

	/**
	 * @brief Check if there is error.
	 * If there is an error, output message in std::cerr.
	 */
	 void CheckError();

	/** Check this effect is on effect-sequences. */
	const bool IsActive() const { return (m_active_count != 0); }

	/** Set this effect will be actived. */
	 void Active();

	/** Set this effect disabled. */
	 void Disable();

private:
	std::array<GLuint, 4> m_frame_buffers{};		/** Frame buffer container */
	std::array<texture_ptr, 4> m_color_buffers{};	/** Color buffer container */
	std::array<GLuint, 8> m_common_buffers{};		/** Universal buffer container */

    ShaderWrapper m_shader_wrapper;

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

private:
	/**
	 * @brief This method gets quad vertex attribute object.
	 * @return Lvalue reference of quad BindingObject shared with all pp frame instance.
	 */
	VertexArrayObject& GetCommonQuadVao();

	/**
	 * @brief This method checks wherther it already has a value on spot you want.
	 * @param[in] id Index to verify.
	 * @param[in] buffer Container to be verified by id.
	 * @return Success or failure flag. Return true if buffer in index is already generated.
	 * (except for helper::CTexture2D. helper::CTexture2D version is below.)
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
	 * @brief Check parameter value is already exist.
	 * @param[in] tag The tag to find and check if it's exist.
	 * @param[in] _Ty Type paramter to check container type in m_paramaters of this.
	 * @return The flag accounts for success or failure of finding one.
	 */
	template <typename _Ty>
	bool IsValueAlreadyExist(const std::string& tag, const _Ty) {
		if (std::is_same_v<float, _Ty>) return m_shader_wrapper.IsValueAlreadyExist<float>(tag);
	}
};

}

#endif /** OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H */
