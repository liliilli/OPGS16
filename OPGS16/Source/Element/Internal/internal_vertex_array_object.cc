#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/internal_vertex_array_object.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
///

/// Header file
#include <Element/Internal/internal_vertex_array_object.h>

#include <GL/glew.h>
/// ::phitos enhanced assertion.
#include <Phitos/Dbg/assert.h>

//!
//! Forward declaration
//!

namespace {
using phitos::type::PtTByte;
} /// unnamed namespace

//!
//! Implementation
//!

namespace opgs16::element::_internal {

int32_t CInternalVertexArrayObject::GetIndiceCount() const noexcept {
  return m_indices_count;
}

int32_t CInternalVertexArrayObject::GetVertexCount() const noexcept {
  return m_vertices_count;
}

CInternalVertexArrayObject::CInternalVertexArrayObject(
    EVboBufferType vbo_buffer_type, PtTByte vbo_buffer_size, int32_t vbo_buffer_length) :
    m_vertices_count(vbo_buffer_length) {
  glGenVertexArrays(1, &m_object.vao_id);
  glBindVertexArray(m_object.vao_id);

  glGenBuffers(1, &m_object.vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_object.vbo_id);

  GLenum buffer_data_type = 0;
  switch (vbo_buffer_type) {
  case EVboBufferType::StaticDraw:  buffer_data_type = GL_STATIC_DRAW; break;
  case EVboBufferType::DynamicDraw: buffer_data_type = GL_DYNAMIC_DRAW; break;
  }

  glBufferData(GL_ARRAY_BUFFER, uint32_t(vbo_buffer_size), nullptr, buffer_data_type);
  glBindVertexArray(0);
}

CInternalVertexArrayObject::CInternalVertexArrayObject(
    EVboBufferType vbo_buffer_type, PtTByte vbo_buffer_size, int32_t vbo_buffer_length,
    EEboBufferType ebo_buffer_type, PtTByte ebo_buffer_size, int32_t ebo_buffer_length) :
    CInternalVertexArrayObject{vbo_buffer_type, vbo_buffer_size, vbo_buffer_length} {
  m_indices_count = ebo_buffer_length;
  glBindVertexArray(m_object.vao_id);
  glGenBuffers(1, &m_object.ebo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object.ebo_id);

  GLenum buffer_data_type = 0;
  switch (ebo_buffer_type) {
  case EEboBufferType::StaticDraw:  buffer_data_type = GL_STATIC_DRAW; break;
  case EEboBufferType::DynamicDraw: buffer_data_type = GL_DYNAMIC_DRAW; break;
  }

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, uint32_t(ebo_buffer_size), nullptr, buffer_data_type);
  glBindVertexArray(0);
}

CInternalVertexArrayObject::CInternalVertexArrayObject(
    const CInternalVertexArrayObject& rhs) : m_object(rhs.m_object) {
  rhs.m_is_resource_moved = true;
  m_indices_count   = rhs.GetIndiceCount();
  m_vertices_count  = rhs.GetVertexCount();
}

CInternalVertexArrayObject& CInternalVertexArrayObject::operator=(
    const CInternalVertexArrayObject& rhs) {
  this->m_object = rhs.m_object;
  rhs.m_is_resource_moved = true;
  m_indices_count   = rhs.GetIndiceCount();
  m_vertices_count  = rhs.GetVertexCount();

  return *this;
}

CInternalVertexArrayObject::~CInternalVertexArrayObject() {
  if (!m_is_resource_moved) {
    if (m_object.ebo_id != 0) {
      glDeleteBuffers(1, &m_object.ebo_id);
    }

    if (m_object.vbo_id != 0) {
      glDeleteBuffers(1, &m_object.vbo_id);
    }

    if (m_object.vao_id != 0) {
      glDeleteVertexArrays(1, &m_object.vao_id);
    }
  }
}

void CInternalVertexArrayObject::Map(EBufferTarget buffer_target,
                                     PtTByte byte_offset,
                                     PtTByte byte_length,
                                     void* start_pointer) {
  if (m_object.vao_id == 0) {
    PHITOS_ASSERT(m_object.vao_id != 0,
        "Object vertex attribute object is not initialized properly.");
    return;
  }

  glBindVertexArray(m_object.vao_id);
  switch (buffer_target) {
  case EBufferTarget::VertexBuffer:
    if (m_object.vbo_id == 0) {
      PHITOS_ASSERT(m_object.vbo_id != 0,
          "Object vertex buffer object is not initialized properly.");
      return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_object.vbo_id);
    BufferMemcpy(GL_ARRAY_BUFFER, byte_offset, byte_length, start_pointer);
    break;
  case EBufferTarget::ElementBuffer:
    if (m_object.ebo_id == 0) {
      return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object.ebo_id);
    BufferMemcpy(GL_ELEMENT_ARRAY_BUFFER,
                 byte_offset, byte_length, start_pointer);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  glBindBuffer(GL_VERTEX_ARRAY, 0);
}

void CInternalVertexArrayObject::BufferMemcpy(GLenum gl_buffer,
                                              const PtTByte& byte_offset,
                                              const PtTByte& byte_length,
                                              void* start_pointer) {
  float* ptr = reinterpret_cast<float*>(
      glMapBufferRange(gl_buffer,
                       static_cast<uint32_t>(byte_offset),
                       static_cast<uint32_t>(byte_length),
                       GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)
  );

  if (ptr == nullptr) {
    PHITOS_ASSERT(ptr != nullptr, "Error mapping buffer object.");
    return;
  }

  memcpy(ptr, start_pointer, static_cast<uint32_t>(byte_length));

  if (const auto result = glUnmapBuffer(gl_buffer);
      result == GL_FALSE) {
    GLenum error = glGetError();
    PHITOS_ASSERT(result != GL_FALSE, "Error unmapping buffer object.");
  }
}

} /// ::opgs16::element::_internal namespace