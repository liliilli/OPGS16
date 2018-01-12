#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

int main() {
    const auto TITLE = "LearnOpenGL";

    /**
     * GLFW 를 쓰기 전에 항상 호출해야만 하는 함수. GLFW library 을 초기화 합니다.
     * 오류의 유무에 따라서 GL_TRUE 와 GL_FALSE 을 반환합니다.
     */
    glfwInit();

    /**
     * glfwWindowHint
     * glfwCreateWindow 함수를 불러오기 앞서서, 타겟 또는 옵션 설정을 바꿀 수 있습니다.
     * target/hint 로 나뉘며 GLFW_ 로 시작하는 열거형 타입과, 해당하는 값으로 갱신합니다.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /**
     * glfwCreateWindow
     * 관련된 컨텍스트와 윈도우를 생성합니다. 
     * 컨텍스트와 윈도우가 어떤 방식으로 만들어지는가는 glfwWindowHint 로 설정을 해야됩니다.
     * 이 때, glfwMakeContextCurrent 로 컨텍스트를 새로 만든 컨텍스트로 갱신해야 됩니다.
     * 
     * width :
     * height :
     * title :
     * monitor : The monitor to use for full screen mode, nullptr to use windowed mode.
     * share : The window whose context to share resoucres with, nullptr to not share
     */
    auto* window = glfwCreateWindow(640, 480, TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /**
     * window 사이즈에 따라서 coordinate 와 data 을 표시할 수 있도록 
     * OpenGL 에게 렌더링 윈도우 사이즈를 알려줘야 합니다. 
     * 따라서 glViewport 로 normalized device coordinates 을 window coordinate 로 맵핑하도록 합니다.
     * 
     * lower_left:
     * lower_bottom:
     * upper_rigth:
     * upper_top:
     *
     * Viewport 을 GLFW 의 dimension 보다 작게 설정함으로써,
     * OpenGL Viewport 밖의 요소들을 GLFW 창 안에 표시하게 할 수도 있습니다.
     */
    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /**
     * GL 프로그램을 실행할 때, 루프를 발생시켜 계속 이미지 등을 렌더링시켜 화면에 표시해야 한다.
     * 이 루프를 Render Loop 라고 합니다.
     *
     * GLboolean glfwWindowShouldClose(GLFWwindow* window)
     * 는 해당 window 가 close signal 을 받았는 지 확인합니다. 만약 아니라면 GL_FALSE
     * 을 반환합니다.
     */
    while (!glfwWindowShouldClose(window)) {
        // Input
        process_input(window);

        // Rendering codes...
        /**
         * Back frame 의 ColorBuffer 의 초기화에 사용될 색을 지정합니다.
         * 이 함수가 실행되고 나서 최대한 빠른 시간 내에 glClear(GL_COLOR_BUFFER_BIT) 가
         * 사용되야 합니다.
         */
        glClearColor(.2f, .3f, .3f, 1.f);
        /**
         * glClear(BIT)
         * 현재 프레임 버퍼 (Back) 의 전체 버퍼 혹은 특정 버퍼를 초기화시킬 수 있습니다.
         * GL_COLOR_BUFFER_BIT 는 Back buffer 을 glClearColor() 의 색으로 초기화힙니다.
         * GL_DEPTH_BUFFER_BIT 는 버퍼 중 Depth buffer 를 초기화 시킵니다.
         * GL_STENCIL_BUFFER_BIT 는 버퍼 중 Stencil Buffer 만을 초기화 시킵니다.
         * 
         * 현재는 색만을 갱신하고 싶기 때문에 GL_COLOR_BUFFER_BIT 을 넣습니다.
         *
         * 이 때, glClearColor 은 glfwWindowHint 와 같이 state-setting function 이며
         * glClear 는 glfwCreateWindow 와 같이 state-using function 임을 알 수 있습니다.
         */
        glClear(GL_COLOR_BUFFER_BIT);

        // Check events and swap the buffers

        /**
         * glfwSwapBuffers(GLFWwindow* window)
         * 일반적인 렌더링 버퍼는 Double buffer 로 구성되어 있습니다.
         * 따라서 glfwSwapBuffer 을 통해서 컬러 버퍼를 프레임마다 바꿔줘야 합니다.
         * Color Buffer 는 GLFW window 의 각각의 픽셀의 색상 값을 저장하고 있습니다.
         *
         * Double Buffer
         * Single Buffer 을 쓰지 않고 Double Buffer 을 쓰는 이유는,
         * 일반적으로 버퍼가 화면에 표시될 때는 한번에 모든 픽셀이 표시되지 않고 한 픽셀씩 표시되면서
         * 최종으로 모든 픽셀이 표시됩니다. 이 때 처리해야할 양이 많아서 1 프레임에 모든 픽셀이 표시
         * 되지 않고, 다음 프레임으로 넘어가기 때문에 화면이 깜빡이는 플리커링 현상이 일어나기 때문입니다.
         * 
         * Double Buffer 의 Front 는 화면에 직접 보여지는 버퍼입니다. 최종 이미지 결과를 담고 있습니다.
         * 대신에 모든 렌더링 코드는 Back Buffer 에서 이뤄집니다. 그래서 Swap 을 통해 프레임마다
         * Front 와 Back 을 바꾸기 때문에 렌더링이 직접 화면에서 보여지지 않으므로
         * 플리커링이 발생하지 않게 됩니다.
         */
        glfwSwapBuffers(window);

        /**
         * glfwPollEvents()
         * 키보드 인풋이나 마우스 움직임과 같은 이벤트가 발생했는지 확인합니다.
         * 이벤트 발생 여부에 따라서 GL 윈도우를 갱신하고 해당되는 콜백 함수를 호출합니다.
         */
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