# 윈도우 리사이즈 비활성화

> 2018-07-23 초안 작성

`OPGS16` `0.1.0` 에서는 GLFW 에서 윈도우를 생성할 때 리사이즈가 가능하도록 했다. 고정 해상도를 지향하기 때문에 절대로 활성화되서는 안된다. 따라서 `GLFW` 에서 제공하는 함수를 사용해서 초기화를 할 때 해상도 변경을 지원하지 않도록 해야한다.

## 구현 사항

* `glfwCreateWIndow()` 전에 `glfwWindowHint` 을 사용해서 설정을 하는 부분에서, `GLFW_RESIZABLE` 을 `GL_FALSE` 로 하여 사이즈가 변경이 안되도록 한다.

## 결과

``` c++
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

리사이징을 비활성화 시켰다.