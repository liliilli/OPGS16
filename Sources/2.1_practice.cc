#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

constexpr auto TITLE = "LearnOpenGL";
constexpr auto SCREEN_WIDTH     = 640u;
constexpr auto SCREEN_HEIGHT    = 480u;
constexpr auto LOG_SIZE         = 0x200u;

constexpr auto vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

constexpr auto fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

GLFWwindow* init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    auto __ = glewInit();

    return window;
}

void shader_err_print(GLuint shader, char* info_log) {
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
}

int main() {
    auto window = init();

    // Setting
    int success = 0;
    char info_log[LOG_SIZE];

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) shader_err_print(vertex_shader, info_log);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) shader_err_print(fragment_shader, info_log);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &success);
    if (!success) shader_err_print(shader_program, info_log);

    // Data
    float vertices[] = {
        -.5f,   -.5f,   +.0f,
        +.0f,   -.5f,   +.0f,
        -.25f,  +.5f,   +.0f,

        +.0f,   -.5f,   +.0f,
        +.5f,   -.5f,   +.0f,
        +.25f,  +.5f,   +.0f
    };

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* const window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}