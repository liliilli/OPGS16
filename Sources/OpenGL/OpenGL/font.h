#ifndef OPENGL_TUTORIAL_FONT_H
#define OPENGL_TUTORIAL_FONT_H

/**
 * @file font.h
 * @brief 
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <ft2build.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include FT_FREETYPE_H

#include "shader.h"

/**
 * @class Font
 * @brief The class manages reading fonts, rendering fonts.
 *
 * This class manages reading fonts, rendering fonts. 
 * default shader is initiated as creating font instance.
 *
 * @todo Shader must be static instance to reduce memory usage.
 * @bug It seems that rendering does not work properly.
 */
class Font {
private:
    /**
     * @struct Chracter
     * @brief Container manages each texture of font glyphes.
     */
    struct Character {
        GLuint      textureID;

        /** glyph information */
        glm::ivec2  size;
        glm::ivec2  bearing;
        GLuint      advance;
    };

    /** Shader program */
    //helper::Shader shader{ "Shaders/font.vert", "Shaders/font.frag" };
    helper::ShaderNew shader{};

public:
    Font(std::string&& font_path);

    /**
     * @brief The method renders given text on given position with given color.
     *
     * @param[in] text String text
     * @param[in] pos Position text has to be shown on. x, y.
     * @param[in] scale Scale factor
     * @param[in] color Color to be colored.
     */
    [[noreturn]] void RenderText(std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color);

private:
    /**
     * @brief The method sets character textures from glyphs and store them to container.
     *
     * This methods called when initiate instance.
     */
    [[noreturn]] void GetCharTextures();

    /**
     * @brief The method sets VAO, VBO to render string on screen.
     *
     * This methods called when initiate instance.
     */
    [[noreturn]] void BindVertexAttributes();

    /**
     * @brief The method separate input to multi-lines strings detecting line-feed return carriage.
     * @param[in] text String text to separate
     * @return string list.
     */
    std::vector<std::string> SeparateTextToList(const std::string text);

    /** Freetype pointer */
    FT_Library freetype;
    FT_Face face;

    // Restrict first 128 characters for now.
    std::unordered_map<GLchar, Character> characters;
    std::array<GLuint, 4> viewport_size;
    glm::mat4 projection;

    GLuint vao, vbo;
};

#endif // OPENGL_TUTORIAL_FONT_H
