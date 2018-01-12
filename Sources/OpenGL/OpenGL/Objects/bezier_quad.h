#ifndef OPENGL_TUTORIAL_BEZIER_QUAD_H
#define OPENGL_TUTORIAL_BEZIER_QUAD_H
#include <array>
#include "../helper.h"
#include "../object.h"
#include "../texture.h"

class BezierQuad : public Object {
public:
    BezierQuad();
    virtual ~BezierQuad() = default;

    /**
     * @brief THe method fucks everything. what a heavy fucking sucks.
     */
    [[noreturn]] virtual void Update() override;

    /**
    * @brief The method calls scene to one objects.
    */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader);

private:
    GLuint bezier_vao{};
    GLuint bezier_vbo{};
    GLuint bezier_ebo{};
    helper::BindingObject quad{};

    texture::Texture2D tex_check = { "Resources/checker.jpg", GL_RGB };

    std::array<GLfloat, 48> m_bezier_patches{
        -1.0f,  -1.0f,  0.0f,
        -0.33f, -1.0f,  0.0f,
        0.33f, -1.0f,  0.0f,
        1.0f,  -1.0f,  0.0f,

        -1.0f,  -0.33f, 0.0f,
        -0.33f, -0.33f, 0.0f,
        0.33f, -0.33f, 0.0f,
        1.0f,  -0.33f, 0.0f,

        -1.0f,   0.33f, 0.0f,
        -0.33f,  0.33f, 0.0f,
        0.33f,  0.33f, 0.0f,
        1.0f,   0.33f, 0.0f,

        -1.0f,   1.0f,  0.0f,
        -0.33f,  1.0f,  0.0f,
        0.33f,  1.0f,  0.0f,
        1.0f,   1.0f,  0.0f,
    };

    std::array<GLuint, 48> m_bezier_indices{
        0, 1, 1, 2, 2, 3,
        4, 5, 5, 6, 6, 7,
        8, 9, 9, 10, 10, 11,
        12, 13, 13, 14, 14, 15,

        0, 4, 4, 8, 8, 12,
        1, 5, 5, 9, 9, 13,
        2, 6, 6, 10, 10, 14,
        3, 7, 7, 11, 11, 15
    };
};

#endif /** OPENGL_TUTORIAL_BEZIER_QUAD_H */