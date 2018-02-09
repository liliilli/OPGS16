#include "bloom_scene.h"
#include "..\Objects\wood_box.h"
#include "..\Objects\quad_floor.h"
#include "..\Objects\light_box.h"

BloomScene::BloomScene() : font{ "Resources/LSANS.TTF" } {
    /** Set shader up */
    InitShaders();

    /** Create Instance to use */
    InitObjects();
    InitLightBoxes();

    /** Bind Framebuffer */
    InitHdrFrameBuffer();
    InitBloomFrameBuffer();

    glGenVertexArrays(1, &empty_vao);
    glEnable(GL_DEPTH_TEST);
}

void BloomScene::InitShaders() {
    using Type = helper::ShaderNew::Type;
    shader.SetShader(Type::VS, "Shaders/Bloom/normal.vert")
        .SetShader(Type::FS, "Shaders/Bloom/normal.frag")
        .Link();

    shader_light.SetShader(Type::VS, "Shaders/Bloom/normal.vert")
        .SetShader(Type::FS, "Shaders/Bloom/lightbox.frag")
        .Link();

    shader_fbo_hdr.SetShader(Type::VS, "Shaders/Bloom/normal.vert")
        .SetShader(Type::FS, "Shaders/Bloom/hdr.frag")
        .Link();

    shader_fullscreen.SetShader(Type::VS, "Shaders/Bloom/full.vert")
        .SetShader(Type::FS, "Shaders/Bloom/full.frag")
        .Link();
    /** bloom (fast gaussian blur) shader */
    shader_fbo_bloom.SetShader(Type::VS, "Shaders/Bloom/full.vert")
        .SetShader(Type::FS, "Shaders/Bloom/bloom.frag")
        .Link();
    /** combining shader */
    shader_final.SetShader(Type::VS, "Shaders/Bloom/full.vert")
        .SetShader(Type::FS, "Shaders/Bloom/combine.frag")
        .Link();
}

void BloomScene::InitObjects() {
    /** Boxes */
    objects[0] = std::make_unique<WoodBox>();
    objects[0]->SetLocalPosition(glm::vec3{ 0, 0, 0 });

    objects[1] = std::make_unique<WoodBox>();
    objects[1]->SetLocalPosition(glm::vec3{ 2, 0, -2 });

    objects[2] = std::make_unique<WoodBox>();
    objects[2]->SetLocalPosition(glm::vec3{ -1, 2, -1 });

    objects[3] = std::make_unique<WoodBox>();
    objects[3]->SetLocalPosition(glm::vec3{ -3, 0, -2 });

    /** Floor */
    objects[4] = std::make_unique<QuadFloor>();
    objects[4]->SetLocalPosition({ 0, -.5f, 0 });
    objects[4]->SetAngle(-90.0f);
    objects[4]->SetRotationFactor({ 1, 0, 0 });
    objects[4]->SetScaleValue(4.0f);
}

void BloomScene::InitLightBoxes() {
    /** Light boxes */
    auto light = std::make_unique<LightBox>();
    light->SetLocalPosition({ -1, 1.5, -3 });
    light->SetScaleValue(.5f);
    light->SetColor({ 1, 1, 1 });
    radiant_objects[0] = std::move(light);

    auto light_2 = std::make_unique<LightBox>();
    light_2->SetLocalPosition({ 0, 1, 2 });
    light_2->SetScaleValue(.5f);
    light_2->SetColor({ 1, 0, 0 });
    radiant_objects[1] = std::move(light_2);

    auto light_3 = std::make_unique<LightBox>();
    light_3->SetLocalPosition({ -4, 0, -1 });
    light_3->SetScaleValue(.5f);
    light_3->SetColor({ 0, 0, 1 });
    radiant_objects[2] = std::move(light_3);
}

void BloomScene::InitHdrFrameBuffer() {
    glGenFramebuffers(1, &hdr_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
    glGenTextures(2, &color_buffer[0]);

    for (size_t i = 0; i < color_buffer.size(); ++i) {
        glBindTexture(GL_TEXTURE_2D, color_buffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 720, 480, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /** Attach texture to framebuffer */
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (int)i,
                               GL_TEXTURE_2D, color_buffer[i], 0);
    }

    /** Make depth buffer for bloom frame buffer */
    GLuint depth_buffer;
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 720, 480);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

    /** Tell OpenGL which color attachments to use for off-screen rendering */
    std::array<GLuint, 2> attachments{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, &attachments[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomScene::InitBloomFrameBuffer() {
    glGenFramebuffers(2, &bloom_fbo[0]);
    glGenTextures(2, &bloom_color_buffer[0]);

    for (size_t i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[i]);
        glBindTexture(GL_TEXTURE_2D, bloom_color_buffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 720, 480, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /** Attach texture to framebuffer */
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, bloom_color_buffer[i], 0);
    }
}

void BloomScene::HandleInput(GLFWwindow * const window) {
    Scene::HandleInput(window);
}

void BloomScene::Draw() {
    glClearColor(.0f, .0f, .0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawHdrBuffer();
    DrawBloomBuffer();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawBloomFinalRender();

    DrawText();
}

void BloomScene::DrawHdrBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_fbo_hdr.Use();

    //auto view = camera.GetViewMatrix();
    //shader_fbo_hdr.SetVecMatrix4f("uView", view);
    //shader_fbo_hdr.SetVecMatrix4f("uProjection", camera.GetProjection());
    //shader_fbo_hdr.SetVec3f("uCameraPos", camera.GetLocalPosition());

    DrawObjects(shader_fbo_hdr);
    /** Lights */
    DrawLightObjects();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomScene::DrawObjects(helper::ShaderNew& shader) {
    /** Set light to shader */ {
        int i = 0;
        for (auto& light : radiant_objects) {
            light.second->SetUpLight(i, shader);
            ++i;
        }
    }
    /** Boxes */
    objects[0]->Draw(shader);
    objects[1]->Draw(shader);
    objects[2]->Draw(shader);
    objects[3]->Draw(shader);
    /** Quad */
    objects[4]->Draw(shader);
}

void BloomScene::DrawLightObjects() {
    shader_light.Use();

    //auto view = camera.GetViewMatrix();
    //shader_light.SetVecMatrix4f("uView", view);
    //shader_light.SetVecMatrix4f("uProjection", camera.GetProjection());

    for (auto& light : radiant_objects) {
        auto item = dynamic_cast<Object*>(light.second.get());
        if (item != nullptr)
            item->Draw(shader_light);
    }
}

void BloomScene::DrawBloomBuffer() {
    bool first_iteration = true;

    shader_fbo_bloom.Use();
    glBindVertexArray(empty_vao);

    for (int i = 0; i < 10; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[i % 2]);
        shader_fbo_bloom.SetBool("horizontal", (((i % 2) == 0) ? true : false));

        glActiveTexture(GL_TEXTURE0);
        if (first_iteration) {
            glBindTexture(GL_TEXTURE_2D, color_buffer[1]);
            first_iteration = false;
        }
        else {
            if (i % 2 == 1)
                glBindTexture(GL_TEXTURE_2D, bloom_color_buffer[0]);
            else
                glBindTexture(GL_TEXTURE_2D, bloom_color_buffer[1]);
        }

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomScene::DrawBloomFinalRender() {
    shader_final.Use();
    shader_final.SetFloat("uExposure", 1.0f);
    glBindVertexArray(empty_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_buffer[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloom_color_buffer[0]);
    /** Render */
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void BloomScene::DrawText() {
    glDisable(GL_DEPTH_TEST);
    font.RenderText("BloomScene", { 25.f, 25.f }, .5f, { 1.f, 1.f, 1.f });
    glEnable(GL_DEPTH_TEST);
}

void BloomScene::Update() {
    for (auto& object : objects) {
        object.second->Update();
    }
}

