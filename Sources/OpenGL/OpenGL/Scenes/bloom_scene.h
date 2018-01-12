#ifndef OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H
#define OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H
#include "..\object.h"
#include "..\shader.h"
#include "..\scene.h"
#include "..\Objects\Interface\i_radiant.h"

class BloomScene : public Scene {
public:
    BloomScene();
    virtual ~BloomScene() = default;

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
    helper::ShaderNew shader{};

    std::unordered_map<int, std::unique_ptr<Object>>    objects;
    std::unordered_map<int, std::unique_ptr<Object>>    radiant_objects;
};

#endif /** OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H */