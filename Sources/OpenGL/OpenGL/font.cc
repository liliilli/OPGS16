#include "font.h"
#include <iostream>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>

Font::Font(std::string&& font_path) 
    : freetype(nullptr), face(nullptr), 
    projection{ glm::ortho(0.f, 720.f, 0.f, 480.f) } {
    
    /** Initiate Shader */ {
        using Type = helper::ShaderNew::Type;
        shader.SetShader(Type::VS, "Shaders/Global/font.vert");
        shader.SetShader(Type::FS, "Shaders/Global/font.frag");
        shader.Link();
    }

    // Check Freetype is well.
    if (FT_Init_FreeType(&freetype)) {
        std::cerr << "ERROR::FREETYPE: Could not init Freetype Library" << std::endl;
    }

    if (FT_New_Face(freetype, font_path.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GetCharTextures();
    FT_Done_Face(face);
    FT_Done_FreeType(freetype);

    BindVertexAttributes();
}

void Font::GetCharTextures() {
    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate Texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        auto width  = face->glyph->bitmap.width;
        auto height = face->glyph->bitmap.rows;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     width, height,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set Texture Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character {
            texture,
            glm::ivec2(width, height),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

        characters.insert(std::make_pair(c, character));
    }
}

void Font::BindVertexAttributes() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::RenderText(std::string input, glm::vec2 input_pos, GLfloat scale, glm::vec3 color) {
    shader.Use();
    shader.SetVec3f("textColor", color);
    shader.SetVecMatrix4f("projection", projection);

    glBindVertexArray(vao);

    /** Separate text to multi lines string */
    auto texts  = SeparateTextToList(input);
    auto pos    = input_pos;

    /** Render texts */
    for (const auto& text : texts) {
        // Iterate through all characters
        for (const auto& chr : text) {
            Character ch_info   = characters.at(chr);

            auto x_offset = ch_info.bearing.x * scale;
            auto y_offset = (ch_info.size.y - ch_info.bearing.y) * scale;
            glm::vec2 ch_pos = pos + glm::vec2{ x_offset, -y_offset };

            auto w = ch_info.size.x * scale;
            auto h = ch_info.size.y * scale;

            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { ch_pos.x,     ch_pos.y + h,   0.0, 0.0 },
                { ch_pos.x,     ch_pos.y,       0.0, 1.0 },
                { ch_pos.x + w, ch_pos.y,       1.0, 1.0 },

                { ch_pos.x,     ch_pos.y + h,   0.0, 0.0 },
                { ch_pos.x + w, ch_pos.y,       1.0, 1.0 },
                { ch_pos.x + w, ch_pos.y + h,   1.0, 0.0 }
            };
            
            // Render texture glyph
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ch_info.textureID);

            // Update content of VBO
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Render
            glDrawArrays(GL_TRIANGLES, 0, 6);
            pos.x += (ch_info.advance >> 6) * scale;
        }

        /** Relocate display position */
        pos.x   = input_pos.x;
        pos.y  -= characters.at(0).size.y;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief The method separate input to multi-lines strings detecting line-feed return carriage.
 * @param[in] text String text to separate
 * @return string list.
 */
std::vector<std::string> Font::SeparateTextToList(const std::string text) {
    std::vector<std::string> result;

    std::stringstream stream{text};
    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}