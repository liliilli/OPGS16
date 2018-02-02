#ifndef OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H
#define OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H

#include "..\Headers\scene_common.h"

class HelloWorld : public Scene {
public:
    HelloWorld();

    [[noreturn]] virtual void HandleInput(GLFWwindow* const window) override;
    [[noreturn]] virtual void Update() override;
    [[noreturn]] virtual void Draw() override;

private:
    FontManager font;

    helper::Shader shad_quad;
    helper::BindingObject quad;

    GLuint ubo;
    std::array<glm::mat4, 2> ubo_data;

    const size_t quad_count = 32;
    GLuint elc_cont[5] = { 6, 1, 0, 0, 0 };
    std::array<glm::mat4, 32> buffer_object;
};

#endif // OPENGL_TUTORIAL_SCENES_HELLO_WORLD_H