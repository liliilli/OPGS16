#ifndef OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H
#define OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file Shader/framebuffer.h
/// @brief Elementary post-processing frame to manage a thing to be rendered.
/// This file consists of application operation class and member API functions.
///
/// @author Jongmin Yun
/// @log
/// 2018-04-20 Add boilerplate codes. Add virtual desctuctor to CFrameBuferFrame.
/// 2018-04-20 Change name of Initiate... functions to Generate... and SetShader().
/// 2018-04-20 Remove error flags and add log output.
/// 2018-04-20 Moved namespace to ::opgs16::element and remove ::shading unknown malicious namespace.
/// 2018-04-21 Rename CPostProcessingFrame to CFrameBufferFrame.
///

#include <array>	/** std::array */
#include <memory>	/** std::shared_ptr */

#include <Shader\shader_wrapper.h>  /// ::opgs16::element::CShaderWrapper
#include <Frame\texture.h>          /// ::opgs16::texture::CTexture2D

#include <opgs16fwd.h>      /// Forward declaration

namespace opgs16::element {

/**
 * @class CFrameBuferFrame
 * @brief The frame manages things to render on post-processing time.
 *
 * This class's m_object_list has frame buffer, texture, and render buffer to be able to bind
 * frame buffer object.
 * And PostProcessingFrame has also shaders to render post-processing effects.
 *
 * Each frame is managed by PostProcessingManager, you can call and bind it through Manager.
 * But you should not try to bind this itself, not Calling PostProcessingManager's BindSequence().
 * This will crash application or at least it incurs undefined behavior.
 *
 * @log
 * 2018-04-20 Moved namespace to ::opgs16::element and remove ::shading unknown malicious namespace.
 * 2018-04-21 Rename CPostProcessingFrame to CFrameBufferFrame.
 */
class CFrameBuferFrame {
public:
    virtual ~CFrameBuferFrame() = default;

    /**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	virtual void Initialize();

	/**
	 * @brief Insert new frame buffer with id value.
	 * If there is already generated frame buffer in id index of m_frame_buffer,
	 * Set flag(ERR_DUPLICATED_FB) up on error variable. Error message can see through calling
	 * CheckError() method.
	 *
	 * @param[in] id The index to create it on container.
	 */
	void GenerateFrameBuffer(const unsigned id);

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
	void GenerateColorBuffer(const unsigned id, GLint internal_format, GLenum format, GLenum type,
                             GLint width = 0, GLint height = 0);

	/** Initialize default depth buffer to [0] position of m_common_buffers. */
	void InitializeDefaultDepthBuffer();

	/**
	 * @brief Get reference of binded texture.
	 * @param[in] id Index to verify.
	 * @return The reference of std::unique_ptr<helper::CTexture2D>.
	 */
	using texture_ptr = std::unique_ptr<opgs16::texture::CTexture2D>;
	texture_ptr& GetTexture(const int32_t id) {
	  return m_color_buffers[id];
	}

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
	 void BindTextureToFrameBuffer(int32_t texture_id,
                                 int32_t framebuffer_id,
                                 GLenum attachment,
                                 GLenum target);

	/**
	 * @brief Create and Initialize shader to be used only in this post-processing frame.
	 * If there is already existed shader, this methods does nothing.
	 *
	 * @param[in] name The name of shader to create.
	 * @param[in] pixel_shader The path of pixel shader.
	 */
	void SetShader(const char* name);

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
	void InsertUniformValue(const char* tag, const _Ty value) {
	  m_shader_wrapper.SetUniformValue(tag, value);
	}

	/**
	 * @brief Replace uniform variable's value with new value.
	 * @param[in] tag The tag to insert value.
	 * @param[in] _Ty Type parameter to compare with supported uniform container's type.
	 * @param[in] value The value insert.
	 */
	template <typename _Ty>
	void ReplaceUniformValue(const char* tag, const _Ty value) {
    m_shader_wrapper.SetUniformValue(tag, value);
	}

private:
	std::array<GLuint, 4> m_frame_buffers{};		/** Frame buffer container */
	std::array<texture_ptr, 4> m_color_buffers{};	/** Color buffer container */
	std::array<GLuint, 8> m_common_buffers{};		/** Universal buffer container */

    CShaderWrapper m_shader_wrapper;

	GLuint empty_vao;
	bool m_is_useable{ false };		/** Must be true to use post-processing instance */

private:
	/**
	 * @brief This method gets quad vertex attribute object.
	 * @return Lvalue reference of quad BindingObject shared with all pp frame instance.
	 */
    CVertexArrayObject& GetCommonQuadVao();

	/**
	 * @brief This method checks wherther it already has a value on spot you want.
	 * @param[in] id Index to verify.
	 * @param[in] buffer Container to be verified by id.
	 * @return Success or failure flag. Return true if buffer in index is already generated.
	 * (except for helper::CTexture2D. helper::CTexture2D version is below.)
	 */
	template <int32_t TAmnt>
	bool IsAlreadyGenerated(const int32_t id,
                          const std::array<GLuint, TAmnt>& buffer) const {
		if (id < TAmnt && buffer[id] == 0)
      return false;

	  return true;
	}

	/// Overriden method of IsAlreadyGenerated<size_t>(const size, const std::array). */
	bool IsAlreadyGenerated(const int32_t id,
                          const decltype(m_color_buffers)& buffer) const {
		if (id < buffer.size() && buffer[id] == nullptr)
      return false;

	  return true;
	}
};

} /*! opgs16::element */

#endif /** OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_FRAME_H */
