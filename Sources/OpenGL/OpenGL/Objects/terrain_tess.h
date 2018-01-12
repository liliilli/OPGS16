#ifndef OPENGL_TUTORIAL_TERRAIN_TESS_H
#define OPENGL_TUTORIAL_TERRAIN_TESS_H
#include "..\helper.h"
#include "..\object.h"
#include "..\texture.h"

class TerrainTess : public Object {
public:
    TerrainTess();
    virtual ~TerrainTess() = default;

    /**
    * @brief THe method fucks everything. what a heavy fucking sucks.
    */
    [[noreturn]] virtual void Update() override;

    /**
    * @brief The method calls scene to one objects.
    */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader);

private:
    helper::BindingObject quad{};
    GLuint vao;

    glm::mat4 mvp;

    texture::Texture2D m_tex_height;
    texture::Texture2D m_tex_color;
};

#endif /** OPENGL_TUTORIAL_TERRAIN_TESS_H */

