#ifndef OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H
#define OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/vao_management.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
///

#include <memory>

#include <Phitos/Enums/found.h>
#include <Phitos/Enums/success.h>

/// ::opgs16::element::_internal::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>

namespace opgs16::manager::_internal::vao {

///
/// @brief
/// Initiate vao management.
/// This function must be called only once.
///
void Initiate();

///
/// @brief
/// Deactivate and release resource of vao.
/// This function must be called only once.
///
void Deactivate();

///
/// @brief
/// Get vao resource with success flag.
///
/// @param[in] vao_name Internal VAO resource instance name you find.
///
/// @return If find, get a pair which left side is weak_ptr of Internal VAO
/// instance which has same name you input and right side is EFound::Found.
/// But otherwise unknown ptr and EFound::NotFound.
///
std::pair<
    std::weak_ptr<element::_internal::CInternalVertexArrayObject>,
    phitos::enums::EFound>
GetVaoResource(const std::string& vao_name);

#ifdef false
///
/// @brief
///
///
/// @param[in] vao_name Internal vao instance name to create.
/// @param[in] vbo_buffer_type
/// @param[in] vbo_buffer_size
///
/// @return If succeed, get a pair which left side is weak_ptr of Internal VAO
/// instance which has same name you create and right side is ESucceed::Succeed.
/// But otherwise unknown ptr and ESucceed::Failed.
///
std::pair<
    std::weak_ptr<element::_internal::CInternalVertexArrayObject>,
    phitos::enums::ESucceed>
SetVaoResource(const std::string& vao_name,
               element::_internal::EVboBufferType vbo_buffer_type,
               phitos::type::PtTByte vbo_buffer_size);

/// ������ VAO �� ����µ�, ���� VAO �� ���� STATIC, DYNAMIC, �� vbo �� ebo ��
/// ������ ������ �˱⳪ �ұ�?
/// ���� 3D �޽��� �������� ������ ���� ���� �����̳ʸ� ����� ����,
/// �̸� Mesh �� ���¾ "�̸�"���� �����ͼ� �ش� "�̸�"�� mesh �����̳ʿ� ���
/// �������� ���� �������� ���� �� �ڿ�������.

///
/// @brief
///
///
#endif


} /// ::opgs16::manager::_internal::vao namespace.

#endif ///OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H