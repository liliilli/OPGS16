#ifndef OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H
#define OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H

#include <string>
#include <unordered_map>
#include <glm\glm.hpp>
#include "..\..\Headers\Fwd\objectfwd.h"    /*! ShaderNew */

/**
 * @file System/Shader/shader_wrapper.h
 * @brief Shader wrapper.
 * ** NEED TO BE TRANSLATED IN ENGLISH **
 * 쉐이더 매니저에서 가져온 쉐이더의 참조 혹은 포인터를 가지며, 이 쉐이더에 갱신될
 * 유니폼 변수들의 집합 객체를 가진다.
 *
 * 이 랩퍼 객체는 _렌더러 혹은 포스트 프로세싱 프레임에 정적으로 붙여질 수 있으며
 * (되도록 스택에 올라갈 수 있도록 붙일 것) Use() 함수와 UpdateUniforms() 를 이용해
 * 쉐이더를 쓰고 유니폼 변수들을 바인딩할 수 있도록 한다.
 *
 * Last updated :: 2018 - 02 - 05
 *
 * @author Jongmin Yun
 */

class ShaderWrapper final {
private:
    struct Paramters {
        std::unordered_map<std::string, float>		m_floats{};
        std::unordered_map<std::string, glm::mat4>	m_mat4s{};
    } m_parameters;

public:
    /**
     * @brief Set shader.
     * @param[in] shader
     */
    [[noreturn]] void SetShader(helper::ShaderNew* const shader);

    /**
     * @brief Use shader.
     * If shader graps nothing, do nothing any more.
     */
    [[noreturn]] void UseShader();

    /**
     * @brief
     */
    [[noreturn]] void CheckError() {};

    /**
     * @brief Insert uniform variable value to be used by shader.
     * @param[in] tag The tag to insert value.
     * @param[in] _Ty Type parameter to compare with supported uniform container's type.
     * @param[in] value The value insert.
     */
    template <typename _Ty>
    [[noreturn]] void InsertUniformValue(const std::string& tag, const _Ty value) {};

    template <>
    void InsertUniformValue<float>(const std::string& tag, const float value) {
        if (!IsValueAlreadyExist(tag, value)) m_parameters.m_floats[tag] = value;
    }

    template <>
    void InsertUniformValue<glm::mat4>(const std::string& tag, const glm::mat4 value) {
        if (!IsValueAlreadyExist(tag, value)) m_parameters.m_mat4s[tag] = value;
    }

    /**
     * @brief Replace uniform variable's value with new value.
     * @param[in] tag The tag to insert value.
     * @param[in] _Ty Type parameter to compare with supported uniform container's type.
     * @param[in] value The value insert.
     */
    template <typename _Ty>
    [[noreturn]] void ReplaceUniformValue(const std::string& tag, const _Ty value) {
        if (IsValueAlreadyExist(tag, value)) {
            _Ty* i = GetIteratorOfSpecifiedPoint<_Ty>(tag);
            *i = value;
        }
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
    }

private:
    template <class T> struct ReturnType { using type = T; };
    template <> struct ReturnType<float> { using type = float; };
    template <> struct ReturnType<glm::mat4> { using type = glm::mat4; };

    /**
     * @brief Searchs and Gets parameter container pointer.
     * @param[in] tag
     * @param[in] _Ty
     * @return Paramter container pointer.(iterator)
     */
    template <typename _Ty>
    typename ReturnType<_Ty>::type* GetIteratorOfSpecifiedPoint(const std::string& tag);

    template <> float* GetIteratorOfSpecifiedPoint<float>(const std::string& tag) {
        return &m_parameters.m_floats.at(tag);
    }

    template <> glm::mat4* GetIteratorOfSpecifiedPoint<glm::mat4>(const std::string& tag) {
        return &m_parameters.m_mat4s.at(tag);
    }

    /**
     * @brief Set uniform variables of shader with new values.
     */
    [[noreturn]] void RefreshUniformValues();
};

#endif /** OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H */