#ifndef OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H
#define OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Shader/shader_wrapper.h
 * @brief Shader wrapper.
 * 쉐이더 매니저에서 가져온 쉐이더의 참조 혹은 포인터를 가지며, 이 쉐이더에 갱신될
 * 유니폼 변수들의 집합 객체를 가진다.
 *
 * 이 랩퍼 객체는 _렌더러 혹은 포스트 프로세싱 프레임에 정적으로 붙여질 수 있으며
 * (되도록 스택에 올라갈 수 있도록 붙일 것) Use() 함수와 UpdateUniforms() 를 이용해
 * 쉐이더를 쓰고 유니폼 변수들을 바인딩할 수 있도록 한다.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-28 Add vector2 uniform value container and function related to vector2.
 * 2018-04-08 Supporting change of shader on running.
 * 2018-04-17 Move ::CShaderWrapper to ::opgs16::element::CShaderWrapper.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <string>
#include <map>

#include <glm\glm.hpp>
#include <GL\glew.h>

#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
namespace element {

class CShaderWrapper {
private:
    struct Paramters {
        std::map<std::string, float> m_floats{};
        std::map<std::string, int> m_ints{};
        std::map<std::string, std::pair<int*, int>> m_int_ptr{};
        std::map<std::string, glm::vec2> m_vec2{};
        std::map<std::string, glm::vec3> m_vec3{};
        std::map<std::string, glm::mat4> m_mat4s{};
    } m_parameters;

public:
    /**
     * @brief Set shader.
     * @param[in] shader
     */
     void SetShader(opgs16::element::CShaderNew* const shader);

    /**
     * @brief Use shader.
     * If shader graps nothing, do nothing any more.
     */
     void UseShader();

    /**
     * @brief Insert uniform variable value to be used by shader.
     * @param[in] tag The tag to insert value.
     * @param[in] _Ty Type parameter to compare with supported uniform container's type.
     * @param[in] value The value insert.
     */
    template <typename _Ty>
    void SetUniformValue(const std::string& tag, const _Ty& value);

    template <>
    void SetUniformValue<float>(const std::string& tag, const float& value) {
        m_parameters.m_floats[tag] = value;
    }

    template <>
    void SetUniformValue<glm::mat4>(const std::string& tag, const glm::mat4& value) {
        m_parameters.m_mat4s[tag] = value;
    }

    template <>
    void SetUniformValue<glm::vec2>(const std::string& tag, const glm::vec2& value) {
        m_parameters.m_vec2[tag] = value;
    }

  template <>
  void SetUniformValue<glm::vec3>(const std::string& var_name, const glm::vec3& value) {
    m_parameters.m_vec3[var_name] = value;
  }

    template <>
    void SetUniformValue<int>(const std::string& tag, const int& value) {
        m_parameters.m_ints[tag] = value;
    }

    void SetUniformValueInt(const char* variable_name, const int value) {
        m_parameters.m_ints[variable_name] = value;
    }

    void SetUniformValueIntPtr(const char* variable_name, int* pointer, const int amount) {
        m_parameters.m_int_ptr[variable_name] = std::make_pair(pointer, amount);
    }
private:
    opgs16::element::CShaderNew* m_shader{ nullptr };// Shader pointer retrieved from ShaderManager.
    bool m_is_useable{ false };				// The flag for preventing using of nullptr shader.

public:
    /**
     * @brief Check parameter value is already exist.
     * @param[in] tag The tag to find and check if it's exist.
     * @param[in] _Ty Type paramter to check container type in m_paramaters of this.
     * @return The flag accounts for success or failure of finding one.
     */
    template <typename _Ty>
    bool IsValueAlreadyExist(const std::string& tag, const _Ty = _Ty{}) {
        if (std::is_same_v<float, _Ty>)
            return m_parameters.m_floats.find(tag) != m_parameters.m_floats.end();
        if (std::is_same_v<glm::mat4, _Ty>)
            return m_parameters.m_mat4s.find(tag) != m_parameters.m_mat4s.end();
        if (std::is_same_v<glm::vec2, _Ty>)
            return m_parameters.m_vec2.find(tag) != m_parameters.m_vec2.end();
        if (std::is_same_v<int, _Ty>)
            return m_parameters.m_ints.find(tag) != m_parameters.m_ints.end();
    }

private:
    /*! Set uniform variables of shader with new values. */
    void RefreshUniformValues();
};

} /*! opgs16::element */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H */