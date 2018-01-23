#ifndef OPENGL_TUTORIAL_SCENES_STARFIELD_POINT_H
#define OPENGL_TUTORIAL_SCENES_STARFIELD_POINT_H
#include "..\object.h"
#include "..\System\Shader\shader.h"
#include "..\scene.h"

class StarField : public Scene {
public:
    StarField();
    virtual ~StarField() = default;

    /**
     * @brief The method handles key codes from window.
     *
     * This methods is called by Application::ProcessInput method.
     *
     * @param[in] window Window handle pointer.
     */
    [[noreturn]] virtual void HandleInput(GLFWwindow* const window) override;

    /**
     * @brief The method update components movement, UI refresh, and so on.
     */
    [[noreturn]] virtual void Update() override;

    /**
     * @brief The method calls scene to draw all objects.
     */
    [[noreturn]] virtual void Draw() override;

private:
    Font font;

    /** ----------------------------------------------------------------------+
     * @comment Initiate methods
     */

    /**
     * @brief Initiate shaders to use in this scene.
     */
    [[noreturn]] void InitShaders();
};


#endif /** OPENGL_TUTORIAL_SCENES_STARFIELD_POINT_H */