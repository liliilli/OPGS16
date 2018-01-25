#ifndef OPENGL_TUTORIAL_TEXTURE_H
#define OPENGL_TUTORIAL_TEXTURE_H

#include <string>
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

public:
    /**
     * @brief   Constructor get path and texture mode as arguments, build and set them.
     * @param   texture_path path to get texture data.
     * @param   bind_mode mode to bind texture as what texture's color data type is.
                bind_mode is limited in GL_RGB, GL_RGBA, and so on.
     */
    Texture2D(const std::string& texture_path, const GLint bind_mode);

    /**
     * \brief  
     * \param
     * \param
     */
    void SetTextureParameterI(const GLint option, const GLint mode);
    
    /**
     * \brief  
     * \param
     */
    void SetTextureParameterI(std::initializer_list<TextureParameter> lists);

    /**
     * \brief  
     */
    const GLuint GetId() const { return texture; }

private:
    mutable GLuint texture;
};

/**
 * @brief   Generate cubemap texture.
 * @details
 * @param   texture_path texture path to load as cubemap texture.<br>
 * texture path should be "{path}/file_name[_dir].type", example "resources/sky.jpg".
 * @return  GLuint cubemap texture id
 *
 * @see     https://learnopengl.com/#!Model-Loading/Model
 */
GLuint LoadCubemap(std::string texture_path);

}

#endif
