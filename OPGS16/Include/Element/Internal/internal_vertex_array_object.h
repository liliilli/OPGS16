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
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
/// 2018-06-14 Add comments.
///

#include <GL/glew.h>
/// ::phitos::type::ptTByte byte type.
#include <Phitos/Types/ptTByte.h>

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
  CInternalVertexArrayObject(EVboBufferType vbo_buffer_type, PtTByte vbo_buffer_size, int32_t vbo_buffer_length);

  CInternalVertexArrayObject(EVboBufferType vbo_buffer_type, PtTByte vbo_buffer_size, int32_t vbo_buffer_length,
                             EEboBufferType ebo_buffer_type, PtTByte ebo_buffer_size, int32_t ebo_buffer_length);

  /// We need customized copy constructor for avoiding duplicated resource
  /// release of VRAM resource.

  CInternalVertexArrayObject(const CInternalVertexArrayObject&);
  CInternalVertexArrayObject& operator=(const CInternalVertexArrayObject&);

  CInternalVertexArrayObject(CInternalVertexArrayObject&&) = default;
  CInternalVertexArrayObject& operator=(CInternalVertexArrayObject&&) = default;

  ~CInternalVertexArrayObject();

  ///
  /// @brief
  /// Mapping function to arbitary buffer chunk to VRAM buffer target.
  ///
  /// @param[in] buffer_target VRAM buffer target to copy buffer chunk.
  /// @param[in] byte_offset Buffer offset byte to start.
  /// @param[in] byte_length Buffer length byte to copy from buffer chunk.
  /// @param[in] start_pointer Start pointer of arbitary buffer chunk.
  ///
  void Map(EBufferTarget buffer_target, PtTByte byte_offset, PtTByte byte_length, void* start_pointer);

  ///
  /// @brief Get vertex array object id. if not initiated, just return 0.
  /// @return this object's vao id.
  ///
  uint32_t GetVaoId() const noexcept {
    return m_object.vao_id;
  }

  ///
  /// @brief Get vertex buffer object id. if not initated, just return 0.
  /// @return this object's vbo id.
  ///
  uint32_t GetVboId() const noexcept {
    return m_object.vbo_id;
  }

  ///
  /// @brief Get element buffer object id. if not initated, just return 0.
  /// @return this object's ebo id.
  ///
  uint32_t GetEboId() const noexcept {
    return m_object.ebo_id;
  }

  ///
  /// @brief Get indices count of this vertex array object.
  ///
  int32_t GetIndiceCount() const noexcept;

  ///
  /// @brief Get vertices count of this vertex array object.
  ///
  int32_t GetVertexCount() const noexcept;

private:
  ///
  /// @brief
  /// Private mapping function for reuseability.
  ///
  /// @param[in] gl_buffer OpenGL buffer type
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
  /// Data container saves vao, vbo and ebo ID.
  ///
  struct DBindingObject final {
    // Vertex Array Object id.
    uint32_t vao_id = 0;
    // Vertex buffer object id which contains vertex information.
    uint32_t vbo_id = 0;
    // Element buffer object id which has vertex order instructs how to draw.
    uint32_t ebo_id = 0;
  };

  DBindingObject  m_object;
  int32_t m_vertices_count  = 0;
  int32_t m_indices_count   = 0;

  mutable bool    m_is_resource_moved = false;
};

} /// ::opgs16::element::internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_INTERNAL_VERTEX_ARRAY_OBJECT_H
