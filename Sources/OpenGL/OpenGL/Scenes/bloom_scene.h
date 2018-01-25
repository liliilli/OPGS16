#ifndef OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H
#define OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H
#include "..\Headers\scene_common.h"
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
    helper::ShaderNew shader_light{};
    helper::ShaderNew shader_fbo_hdr{};
    helper::ShaderNew shader_fullscreen{};
    helper::ShaderNew shader_fbo_bloom{};
    helper::ShaderNew shader_final{};

    /**
     * @comment Variables for high dynamic range frame buffer.
     * Depth buffer is declared as render buffer in body of InitHdrFrameBuffer function.
     */

    GLuint hdr_fbo{};
    std::array<GLuint, 2> color_buffer{};

    /**
     * @comment Variables for bloom (fast gaussian blur) frame buffer.
     */

    /** index 0 is for horizontal, index 1 is for vetical */
    std::array<GLuint, 2> bloom_fbo{};
    /** index 0 is for horizontal, index 1 is for vetical */
    std::array<GLuint, 2> bloom_color_buffer{};

    /** This vao is used in default frame buffer fullscreen quad rendering. */
    GLuint empty_vao{};

    std::unordered_map<int, std::unique_ptr<Object>>    objects;
    std::unordered_map<int, std::unique_ptr<IRadiant>>  radiant_objects;

    /** ----------------------------------------------------------------------+
     * @comment Initiate methods
     */

    /**
     * @brief Initiate shaders to use in this scene.
     */
    [[noreturn]] void InitShaders();

    /**
     * @brief Initiate objects information and instances.
     */
    [[noreturn]] void InitObjects();

    /**
     * @brief Initiate light boxes emit lights.
     */
    [[noreturn]] void InitLightBoxes();

    /**
     * @brief Initiate high dynamic range frame buffer to use.
     */
    [[noreturn]] void InitHdrFrameBuffer();

    /**
     * @brief Initiate gauissan bloom frame buffer to use.
     */
    [[noreturn]] void InitBloomFrameBuffer();

    /** ----------------------------------------------------------------------+
     * @comment sub-draw methods using in Draw() method.
     */

    /**
     * @brief Render all objects to be used in HDR frame buffer.
     */
    [[noreturn]] void DrawHdrBuffer();

    /**
     * @brief Render objects with shader inputted.
     * @param[in] shader Shader to use.
     */
    [[noreturn]] void DrawObjects(helper::ShaderNew& shader);

    /**
     * @brief Render radiant objects.
     */
    [[noreturn]] void DrawLightObjects();

    /**
     * @brief Render hdr color textures with fast gaussian bluring to bloom.
     */
    [[noreturn]] void DrawBloomBuffer();

    /**
     * @brief Render bloom effect texture and combines former default screen buffer.
     */
    [[noreturn]] void DrawBloomFinalRender();

    /**
     * @brief Render string.
     *
     * In this method, at first depth test is disabled and render string to display.
     * At the end depth test is enabled again.
     */
    [[noreturn]] void DrawText();
};

#endif /** OPENGL_TUTORIAL_SCENES_BLOOM_SCENE_H */