#include "helper.h"

#include <stdexcept>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include "..\..\stb_image.h"

namespace helper {

CustomFrameBuffer::CustomFrameBuffer() : mScreenSize{} {
    glGenFramebuffers(1, &frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

    // Not to write color values, set GL_NONE.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Release
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CustomFrameBuffer::GenBuffer(BufferMode mode, const std::string& tag) {
    // If name is already exist, halt program.
    if (buffers.find(tag) != buffers.end()) {
        throw std::runtime_error("Framebuffer, buffer name is already exist");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
    switch (mode) {
    case BufferMode::TEXTURE_2D:
        buffers[tag] = GenTexture2DBuffer();
        break;
    case BufferMode::TEXTURE_CUBE_MAP:
        buffers[tag] = GenTextureCubemapBuffer();
        break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CustomFrameBuffer::Bind() {
    glGetIntegerv(GL_VIEWPORT, &mScreenSize[0]);
    glViewport(0, 0, kWIDTH, kHEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer_id);
}

void CustomFrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(mScreenSize[0], mScreenSize[1], mScreenSize[2], mScreenSize[3]);
}

GLint CustomFrameBuffer::GenTexture2DBuffer() {
    GLuint texture_buffer;

    // Texture buffer Generation
    glGenTextures(1, &texture_buffer);
    glBindTexture(GL_TEXTURE_2D, texture_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, kWIDTH, kHEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    std::array<float, 4> border_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);

    // Binding texture to frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_buffer, 0);

    return texture_buffer;
}

GLint CustomFrameBuffer::GenTextureCubemapBuffer() {
    GLuint texture_cubemap;

    // generate texture buffer
    glGenTextures(1, &texture_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

    for (size_t i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     kWIDTH, kHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Bind texture to frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_cubemap, 0);

    return texture_cubemap;
}

}
