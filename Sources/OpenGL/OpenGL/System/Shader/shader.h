#ifndef OPENGL_TUTORIAL_SHADER_H
#define OPENGL_TUTORIAL_SHADER_H

/**
* @file shader.h
* @brief
*
*
* @author Jongmin Yun
* @version 0.0.2
*/

#include <string>
#include <unordered_map>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "..\..\light.h"

namespace helper {

/**
 * @brief New shader wrapper class
 *
 *
 */
class ShaderNew {
public:
    /**
     * @brief
     */
    enum class Type : int {
        VS  = GL_VERTEX_SHADER,
        TCS = GL_TESS_CONTROL_SHADER,
        TES = GL_TESS_EVALUATION_SHADER,
        GS  = GL_GEOMETRY_SHADER,
        FS  = GL_FRAGMENT_SHADER
    };

    /**
     * @brief
     *
     * @param[in] shader_type
     * @param[in] path
     *
     * @bug When binds const char* code ptr into shader,
     * success flag is not on and go to the error calls but maybe shader program works probablly.
     */
    ShaderNew& SetShader(Type shader_type, const GLchar* path);

    /**
     * @brief
     */
    [[noreturn]] void Link();

    /**
     * @brief
     */
    [[noreturn]] void Use();

    /** Utility uniform functions */

    /**
     * @brief The method sets boolean value to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    [[noreturn]] void SetBool(const std::string& name, bool value) const;

    /**
     * @brief The method sets singed-integer value to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    [[noreturn]] void SetInt(const std::string& name, int value) const;

    /**
     * @brief The method sets float value to arbitary uniform variable.
     *
     * The method sets float (single-precision real) value to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] value value to be inputted.
     */
    [[noreturn]] void SetFloat(const std::string& name, float value) const;

    /**
     * @brief The method sets three float values to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] _1 First value to be defined.
     * @param[in] _2 Second value to be defined.
     * @param[in] _3 Third value to be defined.
     */
    [[noreturn]] void SetVec3f(const std::string& name, const float, const float, const float);

    /**
     * @brief The method sets $ R^3 $ vector to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] vector $ R^3 $ vector refernce l-value to input.
     */
    [[noreturn]] void SetVec3f(const std::string & name, const glm::vec3& vector);

    /**
     * @brief The method sets $ \mathbf{M}_{4x4} $ matrix to arbitary uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] vector $ \mathbf{M}_{4_4} $ matrix reference l-value to input.
     */
    [[noreturn]] void SetVecMatrix4f(const std::string& name, glm::mat4& matrix);

    /**
     * @brief The method sets light::DirectionalLight information to shader uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] container DirectionalLight container instance.
     */
    [[noreturn]] void SetStructDirLight(const std::string& name,
                                        const light::DirectionalLight& container);

    /**
     * @brief The method sets light::PointLight information to shader uniform variable.
     *
     * @param[in] name The name of uniform variable to be specified.
     * @param[in] container PointLight container instance.
     */
    [[noreturn]] void SetStructPointLight(const std::string& name,
                                          const light::PointLight& container);

    /**
     * @brief The method sets light::Spotlight information to shader uniform variable.
     *
     * @param[in] name THe name of uniform variable to be specified.
     * @param[in] container Spotlight container instance.
     */
    [[noreturn]] void SetStructSpotlight(const std::string& name,
                                         const light::Spotlight& container);

private:
    /** Shader's unique id */
    mutable GLuint k_id{};

    /** Boolean Flag. This value must be true when call Shader.Use() */
    bool m_linked{ false };

    /** Compiled Shader list */
    std::unordered_map<Type, GLuint> m_shaders;

    /**
     * @brief
     *
     * @param[in] type
     */
    bool IsCompiled(Type type) {
        return (m_shaders.find(type) != m_shaders.end());
    }

    /**
     * @brief
     *
     * @param[in] path
     */
    std::string ReadShaderCodeFrom(const std::string path);

    /**
     * @brief
     *
     * @param[in] shader
     * @param[in] info_log
     */
    void ShaderErrorPrint(GLuint shader, char* info_log);

    /**
     * @brief
     *
     * @param[in] shader
     * @param[in] info_log
     */
    void LinkingErrorPrint(GLuint shader, char* info_log);
};


/**
 * @brief Old Shader wrapper class
 *
 * @deprecated
 */
class Shader {
public:
    // Costructor reads vertex shader and fragment shader path, and build them.
    Shader(const GLchar* vertex_path, bool link = true);
    Shader(const GLchar* vertex_path, const GLchar* fragment_path, bool link = true);
    Shader(const GLchar* vertex_path, const GLchar* geometry_path, const GLchar* fragment_path);

    // Use / Activate the shader
    void Use();
    // Utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

    void SetVec3f(const std::string& name, const float, const float, const float);
    void SetVec3f(const std::string & name, const glm::vec3& vector);
    void SetVecMatirix4f(const std::string& name, glm::mat4& matrix);

    void SetStructDirLight(const std::string& name, const light::DirectionalLight& container);
    void SetStructPointLight(const std::string& name, const light::PointLight& container);
    void SetStructSpotlight(const std::string& name, const light::Spotlight& container);

    const GLuint GetId() const { return kId; }

private:
    // The program ID
    mutable GLuint kId;
    GLuint vertex_shader;
    GLuint geometry_shader;
    GLuint fragment_shader;

    void ShaderErrorPrint(GLuint shader, char* info_log);
};

}

#endif