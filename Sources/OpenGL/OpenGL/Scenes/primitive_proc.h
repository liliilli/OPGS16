#ifndef OPENGL_TUTORIAL_SCENES_PRIMITIVE_PROC_H
#define OPENGL_TUTORIAL_SCENES_PRIMITIVE_PROC_H

/**
* @file primitive_proc.h
* @brief The file consist of basic scene API.
*
* All derived class based on Scene class can be used parameter of Application::PushScene().
*
* @author Jongmin Yun
* @version 0.0.1
*/

#include "..\Headers\scene_common.h"

class PrimProc : public Scene {
public:
    PrimProc();

    [[noreturn]] virtual void HandleInput(GLFWwindow* const window) override;
    [[noreturn]] virtual void Update() override;
    [[noreturn]] virtual void Draw() override;

private:
    Font font;

    helper::ShaderNew shad_triangle{};
    helper::ShaderNew shad_contour{};
    helper::ShaderNew shad_land{};
    helper::ShaderNew shad_quad{};
    helper::ShaderNew shad_bezier{};

    bool m_is_line{};

    std::unordered_map<int, std::unique_ptr<Object>> objects;

    [[noreturn]] void InitBezierScene();
};

#endif // OPENGL_TUTORIAL_PRIMITVE_PROC_H