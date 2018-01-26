#ifndef OPENGL_TUTORIAL_SCENES_START_H
#define OPENGL_TUTORIAL_SCENES_START_H
#include "..\System\Frame\scene.h"
#include "..\System\Frame\object.h"

class Start : public Scene {
public:
    Start();
    virtual ~Start() = default;

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
    std::unordered_map<int, std::unique_ptr<Object>> objects;

    /** ----------------------------------------------------------------------+
     * @comment Initiate methods
     */

    /** ----------------------------------------------------------------------+
     * @comment sub-draw methods using in Draw() method.
     */

    /**
     * @brief Render string.
     *
     * In this method, at first depth test is disabled and render string to display.
     * At the end depth test is enabled again.
     */
    [[noreturn]] void DrawText();
};

#endif /** OPENGL_TUTORIAL_SCENES_START_H */