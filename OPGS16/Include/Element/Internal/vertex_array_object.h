#ifndef OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H
#define OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
///
/// @file Element/vertex_array_object.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-08 Create file.
/// 2018-04-17 Move ::CVertexArrayObject into ::ogps16::element::CVertexArrayObject.
/// 2018-06-09 Move file to Element/Internal and refine.
///

#include <array>

#include <GL/glew.h>
#include <Phitos/Dbg/assert.h>
#include <Headers/import_logger.h>

namespace opgs16::element {

///
/// @enum EBufferBindingType
///
/// @brief
///
///
enum class EBufferBindingType {
  Once,    // GL_STATIC_DRAW
  Dynamic, // GL_DYNAMIC_DRAW
};

///
/// @enum EVertexBufferType
///
/// @brief
///
///
enum class EVertexBufferType : uint8_t {
  VertexArray   = 0b0000, // VAO
  VertexBuffer  = 0b0010, // VBO
  ElementBuffer = 0b0100  // EBO
};

inline EVertexBufferType operator|(EVertexBufferType lhs, EVertexBufferType rhs) {
  using UnderlyingType = std::underlying_type_t<EVertexBufferType>;

  return static_cast<EVertexBufferType>(
    static_cast<UnderlyingType>(lhs) |
    static_cast<UnderlyingType>(rhs)
  );
}

///
/// @class CVertexArrayObject
///
/// @brief
/// Manages mesh object and release resource automatically.
///
class CVertexArrayObject {
  struct BindingParameter;    /** Forward Declaration */
  using parameter_list = std::initializer_list<BindingParameter>; /** Aliasing */
public:
  CVertexArrayObject(CVertexArrayObject&&) = delete;
  CVertexArrayObject& operator=(CVertexArrayObject&&) = delete;

  CVertexArrayObject(const CVertexArrayObject&) = default;
  CVertexArrayObject& operator=(const CVertexArrayObject&) = default;

  ///
  /// @brief
  ///
  /// @param[in] data
  /// @param[in] stride
  /// @param[in] args
  ///
  template <class TTy, uint32_t TAmnt>
  CVertexArrayObject(
      const std::array<TTy, TAmnt>& data,
      uint32_t stride,
      const parameter_list& args) {
    glGenVertexArrays(1, &m_object.vao_id);
    glBindVertexArray(m_object.vao_id);

    glGenBuffers(1, &m_object.vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_object.vbo_id);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(data),
                 (void*)(&data[0]),
                 GL_STATIC_DRAW);

    const uint32_t stride_bytes = stride * sizeof(TTy);
    for (const auto& arg : args) {
      glVertexAttribPointer(arg.location, arg.count,
          GL_FLOAT, GL_FALSE,
          stride_bytes, (void*)(arg.offset * sizeof(float)));
      glEnableVertexAttribArray(arg.location);
    }

    glBindVertexArray(0);
  }

  ///
  /// @brief
  ///
  template <class TTy, uint32_t TAmnt, class TTy2, uint32_t TAmnt2>
  CVertexArrayObject(
      const std::array<TTy, TAmnt>& data,
      uint32_t stride,
      const parameter_list& args,
      const std::array<TTy2, TAmnt2>& ebo_data) :
      CVertexArrayObject(data, stride, args) {
    glBindVertexArray(m_object.vao_id);

    glGenBuffers(1, &m_object.ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object.ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(ebo_data),
                 (void*)(&ebo_data[0]),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
  }

  /// Must release vertex array object resources of VRAM;
  /// Otherwise introduce memory leak.
  ~CVertexArrayObject() {
    if (m_object.vao_id != 0)
      glDeleteBuffers(1, &m_object.vbo_id);
    if (m_object.ebo_id != 0)
      glDeleteBuffers(1, &m_object.ebo_id);
    if (m_object.vao_id != 0)
      glDeleteVertexArrays(1, &m_object.vao_id);
  }

  ///
  /// @brief
  ///
  /// @param[in] type
  ///
  /// @return
  ///
  inline uint32_t GetBufferId(EVertexBufferType type) const noexcept {
    switch (type) {
    case EVertexBufferType::VertexArray:
      return m_object.vao_id;
    case EVertexBufferType::VertexBuffer:
      return m_object.vbo_id;
    case EVertexBufferType::ElementBuffer:
      return m_object.ebo_id;
    default:
      PHITOS_ASSERT(
          false,
          "EVertexBufferType parmater has only one type value.");
      PUSH_LOG_ERRO("EVertexBufferType pamareter has only one type value.");
      return -1;
    }
  }

private:
  ///
  /// @struct DBindingObject
  ///
  /// @brief
  /// Data container saves id;
  ///
  struct DBindingObject {
    // Vertex Array Object id.
    uint32_t vao_id = 0;
    // Vertex buffer object id which contains vertex information.
    uint32_t vbo_id = 0;
    // Element buffer object id which has vertex order instructs how to draw.
    uint32_t ebo_id = 0;
  };

  DBindingObject m_object;

  ///
  /// @struct BindingParameter
  ///
  /// @brief
  ///
  ///
  struct BindingParameter {
    // Location that specific buffer.
    uint32_t location = 0;
    // Count specific count.
    uint32_t count = 0;
    // Offset
    uint32_t offset = 0;
  };

  ///
  /// @brief
  /// Get offset byte size and return it as void* type.
  ///
  /// @param[in] offset_size
  /// @return offset byte size
  ///
  template <typename _Ty>
  void* GetOffset(const int offset_size) const noexcept {
    return reinterpret_cast<void*>(offset_size * sizeof(_Ty));
  }
};

} /// ::opgs16::element namespace

#endif /// !OPGS16_SYSTEM_FRAME_VERTEX_ARRAY_OBJECT_WRAPPER_H