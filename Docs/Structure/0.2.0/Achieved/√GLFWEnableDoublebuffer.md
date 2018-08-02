# 윈도우 더블버퍼링

> 2018-07-23 초안 작성

`OPGS16` `0.1.0` 에서는 더블버퍼링을 지원하지 않았다. 하지만 `GLFW` 에서는 더블버퍼링을 지원하고 현재 모든 어플리케이션은 더블 버퍼링을 지원하기 때문에 이를 사용해야만 한다.

더블 버퍼링을 사용하지 않고 싱글 버퍼링을 사용하면, 렌더링 속도가 느려질 경우 지터링이 일어날 수 있다는 단점이 존재한다.

## 구현 사항

- `glfwCreateWIndow()` 전에 `glfwWindowHint` 을 사용해서 설정을 하는 부분에서, `GLFW_DOUBLEBUFFER` 을 `GL_TRUE` 로 하여 사이즈가 변경이 안되도록 한다.

## 결과

```c++
// OpenGL Setting
glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
PUSH_LOG_DEBUG("GLFW CONTEXT VERSION 4.3 Core.");

const auto window = glfwCreateWindow(SGlobalSetting::ScreenWidth(),
                                     SGlobalSetting::ScreenHeight(),
                                     application_name.c_str(),
                                     nullptr, nullptr);
```

더블 버퍼링을 활성화 시켰다.