#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include "..\..\GlobalObjects\light.h"

namespace helper {
constexpr unsigned LOG_SIZE = 0x200;

ShaderNew& ShaderNew::SetShader(Type shader_type, const GLchar* path) {
    if (m_linked || IsCompiled(shader_type)) {
        std::cerr << "ERROR::SHADER::ALREADY::COMPILED" << std::endl;
    }

    /** create shader */
    m_shaders[shader_type] = glCreateShader((GLenum)shader_type);

    /** shader setting */
    std::string code = ReadShaderCodeFrom(path);
    auto code_ptr = code.c_str();
    auto source = m_shaders.at(shader_type);

    glShaderSource(source, 1, &code_ptr, nullptr);
    glCompileShader(source);

    /** Log buffer setting to check compilation result of each shader fragment. */
    int success = 0;
    glGetShaderiv(source, GL_COMPILE_STATUS, &success);

    char info_log[LOG_SIZE]{};
    if (!success) ShaderErrorPrint(m_shaders[shader_type], info_log);

    /* Return l-value reference of this, to use it as cascade. */
    return *this;
}

ShaderNew::~ShaderNew() {
    if (!m_shaders.empty())
        m_shaders.clear();

    glDeleteProgram(m_program_id);
}

void ShaderNew::Link() {
    m_program_id = glCreateProgram();

    // Attach shader fragments (already compiled)
    for (auto& shader : m_shaders) {
        glAttachShader(m_program_id, shader.second);
    }

    glLinkProgram(m_program_id);

    for (auto& shader : m_shaders) {
        glDetachShader(m_program_id, shader.second);
        glDeleteShader(shader.second);
    }
    m_shaders.clear();

    int success = 0;
    glGetShaderiv(m_program_id, GL_COMPILE_STATUS, &success);

    char info_log[LOG_SIZE];
    if (!success) ShaderErrorPrint(m_program_id, info_log);

    // Set flag to use Shader for now.
    m_linked = true;
}

void ShaderNew::Use() {
    if (!m_linked) {
        std::cerr << "ERROR::SHADER::NOT::LINKED::YET" << std::endl;
        throw std::runtime_error("ERROR::SHADER::NOT::LINKED::YET");
    }
    else { glUseProgram(m_program_id); }
}

std::string ShaderNew::ReadShaderCodeFrom(const std::string path) {
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream stream;

        // Read file and bring to each stream
        stream << file.rdbuf();
        file.close();

        // Convert stream into plain text string
        code = stream.str();
    }
    catch (std::ifstream::failure err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
        throw std::runtime_error("shader read error" + path);
    }

    return code;
}

void ShaderNew::ShaderErrorPrint(GLuint shader, char* info_log) {
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED" << info_log << std::endl;
}

void ShaderNew::LinkingErrorPrint(GLuint shader, char* info_log) {
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
    std::cout << "ERROR::SHADER::LINKING_FAILED" << info_log << std::endl;
}



void ShaderNew::SetBool(const std::string & name, bool value) const {
    glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), static_cast<GLint>(value));
}

void ShaderNew::SetInt(const std::string & name, int value) const {
    glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), value);
}

void ShaderNew::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_program_id, name.c_str()), value);
}

void ShaderNew::SetVec2f(const std::string& name, const glm::vec2& vector) {
    glUniform2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(vector));
}

void ShaderNew::SetVec3f(const std::string& name,
                         const float _1, const float _2, const float _3) {
    glUniform3f(glGetUniformLocation(m_program_id, name.c_str()), _1, _2, _3);
}

void ShaderNew::SetVec3f(const std::string& name, const glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(vector));
}

void ShaderNew::SetVecMatrix4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.c_str()),
                       1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderNew::SetStructDirLight(const std::string & name,
                                  const light::DirectionalLight & container) {
    SetVec3f(name + ".direction", container.GetDirection());

    SetVec3f(name + ".ambient", container.GetAmbient());
    SetVec3f(name + ".diffuse", container.GetDiffuse());
    SetVec3f(name + ".specular", container.GetSpecular());
}

void ShaderNew::SetStructPointLight(const std::string & name,
                                    const light::PointLight & container) {
    SetVec3f(name + ".position", container.GetLocalPosition());

    SetVec3f(name + ".ambient", container.GetAmbient());
    SetVec3f(name + ".diffuse", container.GetDiffuse());
    SetVec3f(name + ".specular", container.GetSpecular());

    SetFloat(name + ".constant", container.GetConstant());
    SetFloat(name + ".linear", container.GetLinear());
    SetFloat(name + ".quadratic", container.GetQuadratic());
}

void ShaderNew::SetStructSpotlight(const std::string & name,
                                   const light::Spotlight & container) {
        SetVec3f(name + ".direction", container.direction);
    SetVec3f(name + ".position", container.position);

    SetVec3f(name + ".ambient", container.ambient);
    SetVec3f(name + ".diffuse", container.diffuse);
    SetVec3f(name + ".specular", container.specular);

    SetFloat(name + ".inCutoff", container.inCutOff);
    SetFloat(name + ".outCutoff", container.outCutOff);

    SetFloat(name + ".constant", container.constant);
    SetFloat(name + ".linear", container.linear);
    SetFloat(name + ".quadratic", container.quadratic);
}

/**
 * @brief Old Shader wrapper class
 *
 * @deprecated
 */
Shader::Shader(const GLchar * vertex_path, bool link) {
    std::string vertex_code;
    std::ifstream vertex_shader_file;
    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertex_shader_file.open(vertex_path);
        std::stringstream vertex_shader_stream;

        // Read file and bring to each stream
        vertex_shader_stream << vertex_shader_file.rdbuf();
        vertex_shader_file.close();

        // Convert stream into plain text string
        vertex_code = vertex_shader_stream.str();
    }
    catch (std::ifstream::failure err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
    }

    // Setting
    auto vertex_shader_code = vertex_code.c_str();

    // Log buffer setting to check compilation result of each shader program.
    int success = 0;
    char info_log[LOG_SIZE];

    // Vertex Shader compile
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) ShaderErrorPrint(vertex_shader, info_log);

    if (link) {
        // Shader program object creation and linking
        kId = glCreateProgram();
        {
            glAttachShader(kId, vertex_shader);
            glDeleteShader(vertex_shader);
        }
    }

}

Shader::Shader(const GLchar * vertex_path, const GLchar * fragment_path, bool link) {
    // Retrieve the vertex / fragment source code from each file path
    std::string vertex_code;
    std::string fragment_code;

    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    // Ensure ifstream m_object_list can throw exceptions
    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open Files
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);

        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;

        // Read file and bring to each stream
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        // Close file handlers
        vertex_shader_file.close();
        fragment_shader_file.close();

        // Convert stream into plain text string
        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
    }

    // Setting
    auto vertex_shader_code = vertex_code.c_str();
    auto fragment_shader_code = fragment_code.c_str();

    // Log buffer setting to check compilation result of each shader program.
    int success = 0;
    char info_log[LOG_SIZE];

    // Vertex Shader compile
    {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
        glCompileShader(vertex_shader);

        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(vertex_shader, info_log);
    }

    // Fragment Shader compile
    {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(fragment_shader, info_log);
    }

    if (link) {
        // Shader program object creation and linking
        kId = glCreateProgram();
        {
            glAttachShader(kId, vertex_shader);
            glAttachShader(kId, fragment_shader);
            glLinkProgram(kId);

            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            glGetShaderiv(kId, GL_COMPILE_STATUS, &success);
            if (!success) ShaderErrorPrint(kId, info_log);
        }
    }
}

Shader::Shader(const GLchar* vertex_path,
               const GLchar* geometry_path,
               const GLchar* fragment_path) : Shader{ vertex_path, fragment_path, false } {
    std::string geometry_code;

    std::ifstream geometry_shader_file;
    geometry_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        geometry_shader_file.open(geometry_path);

        std::stringstream geometry_stream;
        geometry_stream << geometry_shader_file.rdbuf();

        geometry_shader_file.close();
        geometry_code = geometry_stream.str();
    }
    catch (std::ifstream::failure err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
    }

    // Log buffer setting to check compilation result of each shader program.
    int success = 0;
    char info_log[LOG_SIZE];

    // Shader compile
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    {
        auto code = geometry_code.c_str();
        glShaderSource(geometry_shader, 1, &code, nullptr);
        glCompileShader(geometry_shader);

        glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(geometry_shader, info_log);
    }

    // Linking
    // Shader program object creation and linking
    kId = glCreateProgram();
    {
        glAttachShader(kId, vertex_shader);
        glAttachShader(kId, geometry_shader);
        glAttachShader(kId, fragment_shader);
        glLinkProgram(kId);

        glDeleteShader(vertex_shader);
        glDeleteShader(geometry_shader);
        glDeleteShader(fragment_shader);

        glGetShaderiv(kId, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(kId, info_log);
    }
}

void Shader::Use() {
    // Just use kId to load associated with shader program object.
    glUseProgram(kId);
}

void Shader::SetBool(const std::string & name, bool value) const {
    glUniform1i(glGetUniformLocation(kId, name.c_str()), static_cast<GLint>(value));
}

void Shader::SetInt(const std::string & name, int value) const {
    glUniform1i(glGetUniformLocation(kId, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const {
    glUniform1f(glGetUniformLocation(kId, name.c_str()), value);
}

void Shader::SetVec3f(const std::string & name,
                      const float _1, const float _2, const float _3) {
    glUniform3f(glGetUniformLocation(kId, name.c_str()), _1, _2, _3);
}

void Shader::SetVec3f(const std::string& name, const glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(kId, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::SetVecMatirix4f(const std::string& name,
                             glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(kId, name.c_str()),
                       1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetStructDirLight(const std::string& name, const light::DirectionalLight& container) {
    SetVec3f(name + ".direction", container.GetDirection());

    SetVec3f(name + ".ambient", container.GetAmbient());
    SetVec3f(name + ".diffuse", container.GetDiffuse());
    SetVec3f(name + ".specular", container.GetSpecular());
}

void Shader::SetStructPointLight(const std::string& name, const light::PointLight& container) {
    SetVec3f(name + ".position", container.GetLocalPosition());

    SetVec3f(name + ".ambient", container.GetAmbient());
    SetVec3f(name + ".diffuse", container.GetDiffuse());
    SetVec3f(name + ".specular", container.GetSpecular());

    SetFloat(name + ".constant", container.GetConstant());
    SetFloat(name + ".linear", container.GetLinear());
    SetFloat(name + ".quadratic", container.GetQuadratic());
}

void Shader::SetStructSpotlight(const std::string& name, const light::Spotlight& container) {
    SetVec3f(name + ".direction", container.direction);
    SetVec3f(name + ".position", container.position);

    SetVec3f(name + ".ambient", container.ambient);
    SetVec3f(name + ".diffuse", container.diffuse);
    SetVec3f(name + ".specular", container.specular);

    SetFloat(name + ".inCutoff", container.inCutOff);
    SetFloat(name + ".outCutoff", container.outCutOff);

    SetFloat(name + ".constant", container.constant);
    SetFloat(name + ".linear", container.linear);
    SetFloat(name + ".quadratic", container.quadratic);
}

void Shader::ShaderErrorPrint(GLuint shader, char* info_log) {
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
}

}
