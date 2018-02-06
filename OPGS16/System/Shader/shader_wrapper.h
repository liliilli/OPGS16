#ifndef OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H
#define OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H

#include <unordered_map>
#include <glm\glm.hpp>
#include "shader.h"

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
	 * @brief
	 * @param[in] shader
	 */
	[[noreturn]] void BindShader(helper::ShaderNew* const shader);

	/**
	 * @brief Use shader.
	 * If shader graps nothing, do nothing any more.
	 */
	[[noreturn]] inline void UseShader();

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
	[[noreturn]] void InsertUniformValue<float>(const std::string& tag, const float value) {
		if (!IsValueAlreadyExist(tag, value))
			m_parameters.m_floats[tag] = value;
	}

	template <>
	[[noreturn]] void InsertUniformValue<glm::mat4>(const std::string& tag, const glm::mat4 value) {
		if (!IsValueAlreadyExist(tag, value))
			m_parameters.m_mat4s[tag] = value;
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

private:
	/**
	 * @brief Check parameter value is already exist.
	 * @param[in] tag The tag to find and check if it's exist.
	 * @param[in] _Ty Type paramter to check container type in m_paramaters of this.
	 * @return The flag accounts for success or failure of finding one.
	 */
	template <typename _Ty>
	bool IsValueAlreadyExist(const std::string& tag, const _Ty) {
		if (std::is_same_v<float, _Ty>)
			return m_parameters.m_floats.find(tag) != m_parameters.m_floats.end();
		if (std::is_same_v<glm::mat4, _Ty>)
			return m_parameters.m_mat4s.find(tag) != m_parameters.m_mat4s.end();
	}

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

	template <>
	float* GetIteratorOfSpecifiedPoint<float>(const std::string& tag) {
		return &m_parameters.m_floats.at(tag);
	}

	template <>
	glm::mat4* GetIteratorOfSpecifiedPoint<glm::mat4>(const std::string& tag) {
		return &m_parameters.m_mat4s.at(tag);
	}

	/**
	 * @brief Set uniform variables of shader with new values.
	 */
	[[noreturn]] void RefreshUniformValues() {
		// Float
		for (const auto& item : m_parameters.m_floats)
			m_shader->SetFloat(item.first, item.second);
		// Matrix 4x4
		for (const auto& item : m_parameters.m_mat4s)
			m_shader->SetVecMatrix4f(item.first, item.second);
	}
};

inline void ShaderWrapper::UseShader() {
	if (m_is_useable) {
		m_shader->Use();
		RefreshUniformValues();
	}
}

#endif /** OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H */