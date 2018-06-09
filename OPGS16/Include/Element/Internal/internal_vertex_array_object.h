#ifndef OPGS16_ELEMENT_INTERNAL_INTERNAL_VERTEX_ARRAY_OBJECT_H
#define OPGS16_ELEMENT_INTERNAL_INTERNAL_VERTEX_ARRAY_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/internal_vertex_array_object.h
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
///
/// @todo Need to test.
///

#include <string_view>
/// ::phitos::type::ptTByte byte type.
#include <Phitos/Types/ptTByte.h>
#include <GL/glew.h>

namespace opgs16::element::_internal {

///
/// @enum EVboBufferType
///
/// @brief
/// Vbo buffer data type for using internal routine.
///
enum class EVboBufferType : uint8_t {
  StaticDraw,
  DynamicDraw,
};

///
/// @enum EEboBufferType
///
/// @brief
/// Ebo buffer data type for using internal routine.
///
enum class EEboBufferType : uint8_t {
  StaticDraw,
  DynamicDraw
};

///
/// @enum EBufferTarget
///
/// @brief
/// Specifying buffer target to be mapped.
///
enum class EBufferTarget : uint8_t {
  VertexBuffer,
  ElementBuffer,
};

///
/// @class CInternalVertexArrayObject
///
/// @brief
/// Actual class instance storing vertex array object id, VBO id, EBO id
/// by generate and releasing VRAM resource with RAII.
///
class CInternalVertexArrayObject final {
  using PtTByte = phitos::type::PtTByte;
public:
  CInternalVertexArrayObject(
      std::string_view buffer_name,
      EVboBufferType vbo_buffer_type,
      phitos::type::PtTByte vbo_buffer_size);

  CInternalVertexArrayObject(
      std::string_view buffer_name,
      EVboBufferType vbo_buffer_type,
      phitos::type::PtTByte vbo_buffer_size,
      EEboBufferType ebo_buffer_type,
      phitos::type::PtTByte ebo_buffer_size);

  ~CInternalVertexArrayObject();

  ///
  /// @brief
  ///
  ///
  /// @param[in] buffer_target
  /// @param[in] byte_offset
  /// @param[in] byte_length
  /// @param[in] start_pointer
  ///
  void Map(EBufferTarget buffer_target,
           PtTByte byte_offset, PtTByte byte_length, void* start_pointer);

private:
  ///
  /// @brief
  /// Private mapping function for reuseability.
  ///
  /// @param[in] gl_buffer
  /// @param[in] byte_offset
  /// @param[in] byte_length
  /// @param[in] start_pointer
  ///
  void BufferMemcpy(GLenum gl_buffer,
                    const PtTByte& byte_offset,
                    const PtTByte& byte_length, void* start_pointer);

  ///
  /// @struct DBindingObject
  ///
  /// @brief
  /// Data container saves id;
  ///
  struct DBindingObject final {
    // Vertex Array Object id.
    uint32_t vao_id = 0;
    // Vertex buffer object id which contains vertex information.
    uint32_t vbo_id = 0;
    // Element buffer object id which has vertex order instructs how to draw.
    uint32_t ebo_id = 0;
  };

  const std::string_view m_buffer_name = "";

  DBindingObject m_object;

};

} /// ::opgs16::element::internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_INTERNAL_VERTEX_ARRAY_OBJECT_H
