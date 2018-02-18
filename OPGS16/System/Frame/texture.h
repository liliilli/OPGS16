#ifndef OPENGL_TUTORIAL_TEXTURE_H
#define OPENGL_TUTORIAL_TEXTURE_H

#include <string>
#include <vector>
#include <GL/glew.h>

/**
 * \namespace   texture
 * \brief
 */
namespace texture {

/**
 * \brief       The class for binding Texture id and able to set texture's parameters.
 */
class Texture2D {
private:
	/**
	 * \brief   Inner struct used in class Texture2D methods, SetTextureParameterI.
	 */
	struct TextureParameter {
		GLint option;
		GLint mode;
	};

	struct Size {
		GLint width{}, height{};
	};

public:
	/**
	 * @brief   Constructor get path and texture mode as arguments, build and set them.
	 * @param   texture_path path to get texture data.
	 * @param   bind_mode mode to bind texture as what texture's color data type is.
				bind_mode is limited in GL_RGB, GL_RGBA, and so on.
	 */
	Texture2D(const std::string& texture_path, const GLint bind_mode);

	/**
	 * @brief Create texture with no texture path, but for later use.
	 * @param[in] internal_format
	 * @param[in] format
	 * @param[in] type
	 * @param[in] width
	 * @param[in] height
	 */
	Texture2D(const GLint internal_format, GLenum format, GLenum type,
		GLsizei width = 0, GLsizei height = 0);

	/**
	 * \brief
	 * \param[in]
	 * \param[in]
	 */
	 void SetTextureParameterI(const GLint option, const GLint mode);

	/**
	 * \brief
	 * \param[in]
	 */
	 void SetTextureParameterI(const std::vector<TextureParameter>& lists);

	/**
	 * @brief Set border color of texture.
	 * @param[in]
	 */
	 void SetBorderColor(const std::array<GLfloat, 4>& border_color);

	/**
	 * @brief Get texture id.
	 * @return Texture's Id.
	 */
	const GLuint GetId() const { return texture; }

private:
	mutable GLuint texture;

private:
	/**
	 * @brief Get screen width, height size.
	 * @return Size of screen, width and height.
	 */
	const Size GetSize();
};

}

#endif
