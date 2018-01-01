#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

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

constexpr auto TITLE = "LearnOpenGL";

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    auto __ = glewInit();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex_shader);

    int success = 0;
    char info_log[LOG_SIZE];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, LOG_SIZE, nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, LOG_SIZE, nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_program, 0x200, nullptr, info_log);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /**************************************************************************
     *
     **************************************************************************/

    float vertices[] = {
        -.5f, -.5f, +0.f,
        +.5f, -.5f, +0.f,
        +0.f, +.5f, +0.f
    };

    float __do_not_use_like_this_rectangle_vertices[] = {
        // first triangle
        +.5f, +.5f, +.0f,
        +.5f, -.5f, +.0f,
        -.5f, +.5f, +.0f,
        // second triangle
        +.5f, -.5f, +.0f,
        -.5f, -.5f, +.0f,
        -.5f, +.5f, +.0f
    };

    float rectangle_vertices[] = {
        +.5f, +.5f, +.0f,
        +.5f, -.5f, +.0f,
        -.5f, -.5f, +.0f,
        -.5f, +.5f, +.0f
    };
    unsigned rectangle_indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLuint VBO; 
    GLuint VAO; 
    GLuint EBO; // Element Buffer Object

    /**
     * glGenBuffers(Glsizei size, Gluint* buffers)
     * 한개 혹은 여러개의 버퍼 오브젝트를 생성할 수 있다. 
     * ID 를 통해 유저가 씬 뒤에서 작업하는 버퍼 오브젝트에 접근하도록 할 수 있다.
     * VBO 대신에 VBOs[5] 이런식으로 배열로 만들어서 넘겨줄 수도 있다.
     */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /**
     * glBindBuffer(Glenum target, Gluint buffer)
     * Gen 된 버퍼 오브젝트를 현재 버퍼 타입 타겟에 바인딩한다.
     * 오직 하나의 버퍼 오브젝트만 하나의 타입에 바인딩 될 수 있으며,
     * 2번째 인자에 0 을 집어넣으면 해당 타입을 리셋한다.
     */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /**
     * glBufferData(Type, Size, vertex data, usage)
     * 타입에 바인딩된 버텍스 버퍼 오브젝트에 메모리를 할당하고 버텍스 데이터를 저장한다.
     * 버퍼 오브젝트의 버퍼를 수정하기 위한 함수 중 하나다.
     */
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), 
                 rectangle_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices),
                 rectangle_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering codes...
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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