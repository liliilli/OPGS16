#ifndef OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H
#define OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H

#include "..\scene.h"

class TfFeedback : public Scene {
public:
    TfFeedback();

    [[noreturn]] virtual void HandleInput(GLFWwindow* const window) override;
    [[noreturn]] virtual void Update() override;
    [[noreturn]] virtual void Draw() override;

private:
    Font font;

    helper::Shader m_shad_update;
    helper::Shader m_shad_render;

    helper::Shader shad_quad;
    helper::BindingObject quad;

private:
    static constexpr size_t k_x = 5;
    static constexpr size_t k_y = 5;

    std::array<GLuint, 2> m_vao{};
    std::array<GLuint, 5> m_vbo{};
    std::array<GLuint, 2> m_pos_tbo{};
    unsigned buffer_index{};

    [[noreturn]] void SetInitialNodes();
};

#endif // OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H