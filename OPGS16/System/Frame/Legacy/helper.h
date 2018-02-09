#ifndef OPENGL_TUTORIAL_HELPER_H
#define OPENGL_TUTORIAL_HELPER_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <array>
#include <sstream>
#include <unordered_map>

/**
 * \namespace   helper
 * \brief       Helper namespace to help create less error-proned codes.
 * \details     (Deprecated soon) Helper namespace to help create less error-proned codes.<br>
 *              This contains funcitons binds object with vao, vbo, ebo, a class for making Te
 *              xture2D easily.
 */
namespace helper {

/**
 * \brief
 */
struct BindingObject {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

/**
 * \brief
 */
struct BindingParameter {
    size_t location;
    size_t count;
    size_t offset;
};

/**
 * \brief
 */
template <typename _Ty>
constexpr void* GetOffset(const int i) {
    return (void*)(i * sizeof(_Ty));
}

/**
 * \brief
 */
template <typename _Ty, size_t _Nm>
BindingObject CreateBindingObject(const std::array<_Ty, _Nm>& data, const size_t stride,
                                  const std::initializer_list<BindingParameter> args) {
    BindingObject object{};
    glGenVertexArrays(1, &object.vao);
    glBindVertexArray(object.vao);

    glGenBuffers(1, &object.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), (void*)(&data[0]), GL_STATIC_DRAW);

    const size_t stride_bytes = stride * sizeof(_Ty);
    for (const auto& arg : args) {
        glVertexAttribPointer(arg.location, arg.count, GL_FLOAT, GL_FALSE, stride_bytes,
                              GetOffset<float>(arg.offset));
        glEnableVertexAttribArray(arg.location);
    }

    glBindVertexArray(0);
    return object;
}

/**
 * \brief
 */
template <typename _Ty, size_t _Nm, typename _Ty2, size_t _Nm2>
BindingObject CreateBindingObjectEBO(const std::array<_Ty, _Nm>& data, const size_t stride,
                                     const std::initializer_list<BindingParameter> args,
                                     const std::array<_Ty2, _Nm2> ebo_data) {
    BindingObject object = CreateBindingObject(data, stride, args);
    glBindVertexArray(object.vao);

    glGenBuffers(1, &object.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), (void*)(&ebo_data[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return object;
}

enum class BufferMode {
    TEXTURE_2D,
    TEXTURE_CUBE_MAP
};
//
///**
// * \brief
// */
//class CustomFrameBuffer {
//public:
//    /**
//     * @brief
//     */
//    CustomFrameBuffer();
//
//    /**
//     * @brief    Create user-custom buffer.
//     * @details
//     * @param mode
//     * @param tag
//     * @return  void
//     */
//    [[noreturn]] void GenBuffer(BufferMode mode, const std::string& tag);
//
//    /**
//     * @brief    bind custom framebuffer.
//     * @details
//     * @param
//     * @return  void
//     */
//    [[noreturn]] void Bind();
//
//    /**
//     * @brief    unbind custom framebuffer and bind default framebuffer automatically.
//     * @details
//     * @param
//     * @return  void
//     */
//    [[noreturn]] void Unbind();
//
//    /**
//     * @brief
//     * @details
//     * @param
//     * @return  void
//     */
//    GLuint GetBuffer(const std::string tag) { return buffers.at(tag); }
//
//private:
//    std::unordered_map<std::string, GLuint> buffers;
//    GLuint frame_buffer_id;
//
//    static const unsigned kWIDTH   = 1024;
//    static const unsigned kHEIGHT  = 1024;
//
//    std::array<GLint, 4> mScreenSize;
//
//    GLint GenTexture2DBuffer();
//    GLint GenTextureCubemapBuffer();
//};
//
}

#endif