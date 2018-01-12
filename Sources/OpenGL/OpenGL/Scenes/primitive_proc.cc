#include "primitive_proc.h"

#include "../helper.h"
#include "../constant.h"

#include "..\Objects\bezier_quad.h"
#include "..\Objects\terrain_tess.h"

PrimProc::PrimProc() : font{ "Resources/LSANS.TTF" } {
    /** Set shader */   {
        using Type = helper::ShaderNew::Type;
        shad_triangle.SetShader(Type::VS, "Shaders/Ch8/3_triangle.vert")
            .SetShader(Type::TCS, "Shaders/Ch8/3_triangle.tcs")
            .SetShader(Type::TES, "Shaders/Ch8/3_triangle.tes")
            .SetShader(Type::FS, "Shaders/Ch8/simple_output.frag")
            .Link();

        shad_contour.SetShader(Type::VS, "Shaders/Ch8/3_contour.vert")
            .SetShader(Type::TCS, "Shaders/Ch8/3_contour.tcs")
            .SetShader(Type::TES, "Shaders/Ch8/3_contour.tes")
            .SetShader(Type::FS, "Shaders/Ch8/simple_output.frag")
            .Link();

        shad_land.SetShader(Type::VS, "Shaders/Ch8/3_land.vert")
            .SetShader(Type::TCS, "Shaders/Ch8/3_land.tcs")
            .SetShader(Type::TES, "Shaders/Ch8/3_land.tes")
            .SetShader(Type::FS, "Shaders/Ch8/3_land.frag")
            .Link();

        shad_quad.SetShader(Type::VS, "Shaders/quad.vert")
            .SetShader(Type::FS, "Shaders/quad.frag")
            .Link();
    }

    InitBezierScene();
}

void PrimProc::InitBezierScene() {
    {   /** Set bezier curve curf shader */
        using Type = helper::ShaderNew::Type;
        shad_bezier.SetShader(Type::VS, "Shaders/Ch8/3_bezier.vert")
            .SetShader(Type::TCS, "Shaders/Ch8/3_bezier.tcs")
            .SetShader(Type::TES, "Shaders/Ch8/3_bezier.tes")
            .SetShader(Type::FS, "Shaders/Ch8/3_bezier.frag")
            .Link();
    }
    
    objects[0] = std::make_unique<BezierQuad>();
    objects[1] = std::make_unique<TerrainTess>();
}

void PrimProc::HandleInput(GLFWwindow* const window) {
    if (DoesKeyPressed(window, GLFW_KEY_0))
        m_is_line = !m_is_line;
    
    Scene::HandleInput(window);
}

void PrimProc::Update() {
    for (auto& object : objects)
        object.second->Update();

    camera.Refresh();
}

void PrimProc::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering objects
    glEnable(GL_DEPTH_TEST);

    if (!m_is_line)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shad_land.Use();
    objects[1]->Draw(shad_land);

    //shad_bezier.Use();
    //objects[0]->Draw(shad_bezier);

    // Draw Text (UI)
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    font.RenderText("PrimProc", { 25.f, 25.f }, 1.0f, { 1.f, 1.f, 1.f });
}
