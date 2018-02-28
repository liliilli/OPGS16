#ifndef OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H
#define OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H

#include <string>
#include <unordered_map>
#include <map>
#include <glm\glm.hpp>
#include "..\..\Headers\Fwd\objectfwd.h"    /*! ShaderNew */

/**
 * @file System/Shader/shader_wrapper.h
 * @brief Shader wrapper.
 * ** NEED TO BE TRANSLATED IN ENGLISH **
 * ���̴� �Ŵ������� ������ ���̴��� ���� Ȥ�� �����͸� ������, �� ���̴��� ���ŵ�
 * ������ �������� ���� ��ü�� ������.
 *
 * �� ���� ��ü�� _������ Ȥ�� ����Ʈ ���μ��� �����ӿ� �������� �ٿ��� �� ������
 * (�ǵ��� ���ÿ� �ö� �� �ֵ��� ���� ��) Use() �Լ��� UpdateUniforms() �� �̿���
 * ���̴��� ���� ������ �������� ���ε��� �� �ֵ��� �Ѵ�.
 *
 * @author Jongmin Yun
 * @date 2018-02-28
 *
 * @log
 * 2018-02-28 Add vector2 uniform value container and function related to vector2.
 */

class ShaderWrapper final {
private:
    struct Paramters {
        std::map<std::string, float>		m_floats{};
        std::map<std::string, glm::vec2>    m_vec2{};
        std::map<std::string, glm::mat4>	m_mat4s{};
    } m_parameters;

public:
    /**
     * @brief Set shader.
     * @param[in] shader
     */
     void SetShader(helper::ShaderNew* const shader);

    /**
     * @brief Use shader.
     * If shader graps nothing, do nothing any more.
     */
     void UseShader();

    /**
     * @brief
     */
     void CheckError() {};

    /**
     * @brief Insert uniform variable value to be used by shader.
     * @param[in] tag The tag to insert value.
     * @param[in] _Ty Type parameter to compare with supported uniform container's type.
     * @param[in] value The value insert.
     */
    template <typename _Ty>
    void InsertUniformValue(const std::string& tag, const _Ty& value);

    template <>
    void InsertUniformValue<float>(const std::string& tag, const float& value) {
        if (!IsValueAlreadyExist(tag, value)) m_parameters.m_floats[tag] = value;
    }

    template <>
    void InsertUniformValue<glm::mat4>(const std::string& tag, const glm::mat4& value) {
        if (!IsValueAlreadyExist(tag, value)) m_parameters.m_mat4s[tag] = value;
    }

    template <>
    void InsertUniformValue<glm::vec2>(const std::string& tag, const glm::vec2& value) {
        if (!IsValueAlreadyExist(tag, value)) m_parameters.m_vec2[tag] = value;
    }

    /**
     * @brief Replace uniform variable's value with new value.
     * @param[in] tag The tag to insert value.
     * @param[in] _Ty Type parameter to compare with supported uniform container's type.
     * @param[in] value The value insert.
     */
    template <typename _Ty>
    void ReplaceUniformValue(const std::string& tag, const _Ty& value);

    template <>
    void ReplaceUniformValue<glm::mat4>(const std::string& tag, const glm::mat4& value) {
        m_parameters.m_mat4s.at(tag) = value;
    }

    template <>
    void ReplaceUniformValue<float>(const std::string& tag, const float& value) {
        m_parameters.m_floats.at(tag) = value;
    }

    template <>
    void ReplaceUniformValue<glm::vec2>(const std::string& tag, const glm::vec2& value) {
        m_parameters.m_vec2.at(tag) = value;
    }

private:
    helper::ShaderNew* m_shader{ nullptr };	// Shader pointer retrieved from ShaderManager.
    bool m_is_useable{ false };				// The flag for preventing using of nullptr shader.

    enum class ErrorType {
        OK,
        SHADER_IS_NULLPTR,
    } m_error{ ErrorType::OK };

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
    }

private:
    /*! Set uniform variables of shader with new values. */
    void RefreshUniformValues();
};

#endif /** OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H */