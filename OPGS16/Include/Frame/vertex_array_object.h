#ifndef OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H
#define OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System\Frame\vertex_array_object.h
 * @brief
 * @log
 * 2018-02-08 Create file.
 * 2018-04-17 Move ::CVertexArrayObject into ::ogps16::element::CVertexArrayObject.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <array>        /*! std::array */
#include <GL\glew.h>    /*!  */

namespace opgs16 {
namespace element {
/*!
 * @class CVertexArrayObject
 * @brief
 */
class CVertexArrayObject {
private:
    struct BindingParameter;    /** Forward Declaration */
    using parameter_list = std::initializer_list<BindingParameter>; /** Aliasing */

public:
    /*!
     * @brief
     */
    template <class _Ty, size_t _Amnt>
    CVertexArrayObject(const std::array<_Ty, _Amnt>& data, const size_t stride,
                      const parameter_list& args);

    /*!
     * @brief
     */
    template <class _Ty, size_t _Amnt, class _Ty2, size_t _Amnt2>
    CVertexArrayObject(const std::array<_Ty, _Amnt>& data, const size_t stride,
                      const parameter_list& args,
                      const std::array<_Ty2, _Amnt2>& ebo_data);

    /*!
     * @brief
     * @return
     */
    inline const unsigned GetVaoId() const {
        return m_object.vao;
    }

private:
    /*!
     * @brief
     */
    struct BindingObject {
        unsigned vao;   /*! Vertex Array Object id. */
        unsigned vbo;   /*! Vertex buffer object id, which contains vertex information. */
        unsigned ebo;   /*! Element buffer object which has vertex order instructs how to draw. */
    } m_object;

    /*!
     * @brief
     */
    struct BindingParameter {
        size_t location;    /*! Location that specific buffef . */
        size_t count;       /*! Count specific count */
        size_t offset;      /*! Offset */
    };

private:
    /*!
     * @brief Get offset byte size and return it as void* type.
     * @param[in] offset_size
     * @return offset byte size
     */
    template <typename _Ty>
    void* GetOffset(const int offset_size) {
        return (void*)(offset_size * sizeof(_Ty));
    }
};

template<typename _Ty, size_t _Amnt>
inline CVertexArrayObject::CVertexArrayObject(const std::array<_Ty, _Amnt>& data,
                                            const size_t stride,
                                            const parameter_list & args) {
    glGenVertexArrays(1, &m_object.vao);
    glBindVertexArray(m_object.vao);

    glGenBuffers(1, &m_object.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_object.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), (void*)(&data[0]), GL_STATIC_DRAW);

    const size_t stride_bytes = stride * sizeof(_Ty);
    for (const auto& arg : args) {
        glVertexAttribPointer(arg.location, arg.count,
                              GL_FLOAT, GL_FALSE,
                              //stride_bytes, GetOffset<float>(arg.offset));
                              stride_bytes, (void*)(arg.offset * sizeof(float)));
        glEnableVertexAttribArray(arg.location);
    }

    glBindVertexArray(0);
}

template<class _Ty, size_t _Amnt, class _Ty2, size_t _Amnt2>
inline CVertexArrayObject::CVertexArrayObject(const std::array<_Ty, _Amnt>& data,
                                            const size_t stride,
                                            const parameter_list & args,
                                            const std::array<_Ty2, _Amnt2>& ebo_data) :
    CVertexArrayObject(data, stride, args) {
    glBindVertexArray(m_object.vao);

    glGenBuffers(1, &m_object.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(ebo_data), (void*)(&ebo_data[0]),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
}

} /*! opgs16::element */
} /*! opgs16*/

#endif // !OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H