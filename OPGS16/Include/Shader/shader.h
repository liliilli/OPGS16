#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_SHADER_H
#define OPGS16_SYSTEM_ELEMENT_PUBLIC_SHADER_H

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
 * @file System/Shader/shader.h
 * @file System/Element/Public/shader.h
 * @brief Shader program
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-26 Undefined
 * 2018-02-28 Add SetVec2f();
 * 2018-03-12 Refactoring.
 * 2018-04-08 Remove old shader class. Add SetVecIntPtr method on CShaderNew
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <unordered_map>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <opgs16fwd.h> /// Forward declaration

namespace opgs16 {
namespace element {

/*!
 * @class CShaderNew
 * @brief New shader wrapper class
 */
class CShaderNew {
public:
    /*!
     * @brief
     * @param[in] shader_type
     * @param[in] path
     *
     * @bug When binds const char* code ptr into shader,
     * success flag is not on and go to the error calls but maybe shader program works probablly.
     */
    CShaderNew& SetShader(opgs16::resource::EShaderType shader_type, const GLchar* path);

    virtual ~CShaderNew();

    /**
     * @brief
     */
    void Link();

    /**
     * @brief
     */
    void Use();

  ///
  /// @brief Get shader program id.
  /// @return Program id.
  ///
  uint32_t GetProgramId() const noexcept {
    return static_cast<uint32_t>(m_program_id);
  }

    /**---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
     * Utility uniform functions
     *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

    /**
     * @brief The method sets boolean value to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    void SetBool(const std::string& name, bool value) const;

    /**
     * @brief The method sets singed-integer value to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    void SetInt(const std::string& name, int value) const;

    /*!
     * @brief Set signed-integer value C-array pointer with amount to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value Value to be inputted.
     * @param[in] amount Amount how many values to be inputted.
     */
    void SetIntPtr(const std::string& name, int* value, int amount) const;

    /**
     * @brief The method sets float value to arbitary uniform variable.
     * The method sets float (single-precision real) value to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    void SetFloat(const std::string& name, float value) const;

    /**
     * @brief The method sets $ R^2 $ vector to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] vector $ R^2 $ vector refernce l-value to input.
     */
    void SetVec2f(const std::string& name, const glm::vec2& vector);

    /**
     * @brief The method sets three float values to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] _1 First value to be defined.
     * @param[in] _2 Second value to be defined.
     * @param[in] _3 Third value to be defined.
     */
    void SetVec3f(const std::string& name, const float, const float, const float);

    /**
     * @brief The method sets $ R^3 $ vector to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] vector $ R^3 $ vector refernce l-value to input.
     */
    void SetVec3f(const std::string & name, const glm::vec3& vector);

    /**
     * @brief The method sets $ \mathbf{M}_{4x4} $ matrix to arbitary uniform variable.
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] vector $ \mathbf{M}_{4_4} $ matrix reference l-value to input.
     */
    void SetVecMatrix4f(const std::string& name, const glm::mat4& matrix);

    ///**
    // * @brief The method sets light::DirectionalLight information to shader uniform variable.
    // * @param[in] name The name of uniform variable to be specified.
    // * @param[in] container DirectionalLight container instance.
    // */
    //void SetStructDirLight(const std::string& name, const light::DirectionalLight& container);

    ///**
    // * @brief The method sets light::PointLight information to shader uniform variable.
    // * @param[in] name The name of uniform variable to be specified.
    // * @param[in] container PointLight container instance.
    // */
    // void SetStructPointLight(const std::string& name, const light::PointLight& container);

    ///**
    // * @brief The method sets light::Spotlight information to shader uniform variable.
    // * @param[in] name THe name of uniform variable to be specified.
    // * @param[in] container Spotlight container instance.
    // */
    // void SetStructSpotlight(const std::string& name, const light::Spotlight& container);

protected:
    mutable GLuint m_program_id{};  /*! Unique id of shader program */
    bool m_linked{ false }; /*! Boolean Flag. This value must be true when call Shader.Use() */
    std::unordered_map<opgs16::resource::EShaderType, unsigned> m_shaders; /*! Compiled Shader list */

    /**
     * @brief
     * @param[in] type
     */
    inline bool IsCompiled(opgs16::resource::EShaderType type) const {
        return m_shaders.find(type) != m_shaders.end();
    }

    /**
     * @brief
     * @param[in] path
     */
    std::string ReadShaderCodeFrom(const std::string path);

    /**
     * @brief
     * @param[in] shader
     * @param[in] info_log
     */
    void ShaderErrorPrint(GLuint shader, char* info_log);

    /**
     * @brief
     * @param[in] shader
     * @param[in] info_log
     */
    void LinkingErrorPrint(GLuint shader, char* info_log);
};

} /*! opgs16::element */
} /*! opgs16 */

#endif /*! OPGS16_SYSTEM_ELEMENT_PUBLIC_SHADER_H */