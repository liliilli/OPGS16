# Chapter 7. Camera

오브젝트의 임의 Local 좌표를 NDC 로 변환하는 과정에서, World 공간을 *카메라* 의 시점으로 전환하는 **View matrix** 가 존재한다는 것을 알 수 있었다. 이 *카메라* 의 시점을 조절하면 우리가 직접 World 공간 상에서 움직인다는 느낌을 줄 수 있다.

## Camera / View space

Camera 혹은 View 공간에 대해서 얘기를 할 때, 카메라의 시점에서 본 모든 정점 좌표들을 가상 공간의 원점들의 집합으로 볼 수 있다. View matrix 는 World 공간의 좌표들을 View 좌표들로 변환시키는데, 이 때 작용하는 요소가 **카메라의 위치** 와 **카메라의 방향** 이다.

따라서 *카메라* 자체를 정의하기 위해서는 World 공간의 좌표와, 그 좌표에서 어디 방향을 바라보고 있는지도 필요하며, *카메라* 자체의 방향 전환에 따른, 카메라를 원점으로 하는 별도의 3 개의 축도 필요하다. 

![img](https://learnopengl.com/img/getting-started/camera_axes.png)

### Camera Position

카메라 위치를 변경하는 것은 쉽다. 카메라 위치는, 카메라가 어디에 있는지 알려주는 World 공간의 *벡터* 이다. **카메라 자체의 좌표계**도 만들어줘야 하는데 이 과정을 지금 하려고 한다.

우선은 카메라 자체의 좌표공간이 아닌, World 공간에 대해 이동을 시킨다.

``` c++
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  
```

### Camera direction

다음으로 필요한 것은, 카메라가 어디를 바라볼지 정하는 것이다. 여기서는 카메라가 바라볼 좌표를 `(0, 0, 0)` 이라고 둔다. 그러면 현재 World 공간 상에서의 카메라의 좌표와 바라볼 지점을 뺌으로써, **바라볼 방향의 벡터의 반대 방향의 벡터** 를 얻게 된다. 이 때 얻은 벡터는 Normalize 시킨다. 이렇게 함으로써 y 축 혹은 z 축을 얻게 된다.

``` c++
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
```

### Right axis

이제 카메라 자체 좌표계의 좌/우 이동 축 (`x` 축) 을 만든다. 여기서 **외적**이 쓰인다. World 좌표의 y 축 유닛 벡터와 이전에 구한 `cameraDirection` (y 축 혹은 z 축 벡터) 를 외적해서, 노멀라이즈 시키면 `x` 축의 기준이 되는 유닛 벡터가 만들어진다.

``` c++
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
```

### Up axis

이제 `x` 축과 `z` 축이 만들어 졌으므로, `y` 축의 기준이 되는 유닛 벡터만 구하면 된다. 이 역시 **외적**으로 해결한다.

``` c++
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
```

이 과정은 [Gram-Schmidt Process](https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process) 라고 부르는, 3차원 좌표 공간에 대해서 orthonormal 한 set 을 구하는 과정이다.

## Look At

`glm::lookAt()` 함수는 World 공간 상에서의 카메라의 이동 위치, 바라볼 지점, 기준이 되는 `UpVector` 을 인자로 받아 타겟 지점을 바라보는 **View Matrix** 을 만든다. 이 함수는, 위에서 구한 카메라의 `x` `y` `z` 축에 해당되는 벡터를 이용해 World 공간에 위치한 어느 벡터라도 맵핑을 해서 Camera 공간 좌표로 변환시킬 수 있다.
$$
LookAt =
\begin{bmatrix} {R}_x & R_y & R_z & 0 \\ U_x & U_y & U_z & 0 \\ D_x & D_y & D_z & 0 \\ 0 & 0 & 0 & 1 \end{bmatrix} 
* 
\begin{bmatrix} 1 & 0 & 0 & -P_x \\ 0 & 1 & 0 & -P_y \\ 0 & 0 & 1 & -P_z \\ 0 & 0 & 0 & 1 \end{bmatrix}
$$
**R** 벡터는 Right vector 이고, **U** 벡터는 Up vector, **D** 는 Direction vector 이며, **P** 는 World 좌표 상에서의 카메라 위치 벡터이다. 여기서 주의해야 할 점은, 카메라 위치 벡터의 방향이 반대 방향이라는 것이다. 왜냐면 기존의 방향은 카메라가 바라볼 방향의 반대 방향이기 때문이다. 

`glm` 은 `LookAt`을 지원하고 있기에, 다음과 같이 써서 View matrix 을 만들 수 있다.

``` c++
glm::mat4 view;
view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, 1.0f, 0.0f));
```

`glm::lookAt` 함수는 차례대로, $ P $ (이동 위치), $ D $ (바라볼 위치), $ Y $ (양수 y 방향 벡터) 를 인자로 받는다. 

## Walk Around

이제 키 입력을 받아서 카메라를 직접 움직여 보자. 우선 먼저 해야할 것은 카메라의 위치, 카메라가 바라볼 지점, 그리고 카메라의 세가지 축을 만드는데 기준이 되는 `+y` 벡터를 만들어 저장하는 것이다.

``` c++
auto g_pos  = glm::vec3(0, 0, 3);
auto g_dest = glm::vec3(0, 0,-1);
const auto g_posy = glm::vec3(0, 1, 0);

// 그리고 lookAt function 은 다음과 같이 고친다.
m_view = glm::lookAt(g_pos, g_pos + g_dest, g_posy);
```

키보드 입력을 받는 콜백 함수 `process_input` 에서 WASD 키를 누를 때 카메라가 이동할 수 있도록 한다.

``` c++
void process_input(GLFWwindow* const window) {
    float camera_speed = 0.0005f;

    if (DoesKeyPressed(window, GLFW_KEY_W))
        g_pos += camera_speed * g_dest;
    else if (DoesKeyPressed(window, GLFW_KEY_S))
        g_pos -= camera_speed * g_dest;
    else if (DoesKeyPressed(window, GLFW_KEY_A))
        g_pos -= glm::normalize(glm::cross(g_dest, g_posy)) * camera_speed;
    else if (DoesKeyPressed(window, GLFW_KEY_D))
        g_pos += camera_speed * glm::normalize(glm::cross(g_dest, g_posy));
}
```

여기서 좌우로 움직일 때는 명시적으로 외적 벡터 (카메라를 원점으로 한 x 축 벡터)를 평준화시켜야 한다.  왜냐면 `g_dest` 나 `g_posy` 의 값에 따라서 외적 벡터로 나온 x 축 벡터 값의 크기가 *1을 넘을 수도* 있기 때문이다.

그리고 이동 속도를 표준화 시키기 위해 각 렌더 프레임을 가져와 카메라 속도를 보정시킨다.

``` c++
float deltaTime = 0.0f;	// Time between cf, lf
float lastFrame = 0.0f; // Time of last frame

//...

float currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;

//...

void processInput(GLFWwindow *window)
{
  float cameraSpeed = 2.5f * deltaTime;
  ...
}
```



![img](./7_1.gif)

## Look around (마우스로 공간 돌아보기)

마우스를 이용해서 공간을 직접 돌아다니면서 바라보는 시점을 자유자재로 변경하기 위해서는, `g_dest` 벡터의 수치를 바꿔야 한다. 하지만 마우스 회전에 따라서 `g_dest` 벡터의 수치를 바꾸는 것은 약간의 생각을 요한다.

### Euler angles (오일러 각)

**오일러 각**은 3D 공간 상에서 무작위 회전을 표현할 수 있는 3개의 각도이다. 다음 3가지 각도는 각각의 이름을 가진다.

1. `x` 축에 대한 시계 방향 회전 : `pitch` : 위, 아래로 얼마나 기울었는가
2. `y` 축에 대한 시계 방향 회전 : `yaw` : 좌, 우로 얼마나 회전했는가
3. `x` 축에 대한 시계 방향 회전 : `roll` : 동체가 얼마나 기울어졌는가

![img](https://learnopengl.com/img/getting-started/camera_pitch_yaw_roll.png)

항공 시뮬레이터 등을 만들지 않는 한, (또는 특정 시점을 구현하려고 하지 않는 한) 왠만해선 `pitch` 와 `yaw` 만을 쓸 것이다. 이제 어디를 바라보고 있는 지를 저장하는 direction vector 을 구하기 위해 계산을 해보자.

![img](https://learnopengl.com/img/getting-started/camera_pitch.png)

먼저, `x/z` 평면이 밑에 있고 `y` 축이 수직에 있을 때 마우스를 **위 / 아래로** 움직여서 `pitch` 값이 발생했다고 본다. 그러면 다음과 같이 `xyz` 축이 영향을 받게 된다.

``` c++
direction.y = sin(glm::radians(pitch));
// x/z 평면의 x, z 축도 영향을 받는다.
direction.x = cos(glm::radians(pitch));
direction.z = cos(glm::radians(pitch));
```

이제 `y` 축에 대한 회전 각도인 `yaw`를 반영할 차례이다.

![img](https://learnopengl.com/img/getting-started/camera_yaw.png)

만약 `pitch` 값 만을 반영한 direction vector 을 View matrix 에 적용한다면, 위로 치켜 들거나 아래로 숙였을 때 `x` 값과 `z` 값은 매우 요동칠 것이다. 따라서 위로만 치켜들거나 아래로만 숙일 때는 `x` 와 `z` 값이 거의 0 에 근접할 수 있도록, `yaw` 값을 곱해서 반영해야 한다.

``` c++
direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
direction.y = sin(glm::radians(pitch));
direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
```

이제 `pitch` 와 `yaw` 값을 마우스 입력을 통해 받아들이기만 하면 된다.

### Mouse input

마우스를 좌우로 움직이는 것으로 `yaw` 값을 줄 수 있고, 위아래로 움직이는 것으로 `pitch` 값을 줄 수 있다. 기본적인 아이디어는, 각 프레임마다 마우스의 위치를 저장해서 다음 프레임에 마우스의 갱신된 위치를 뺀 차이를 이용해 `yaw` 와 `pitch` 을 반환해 반영하는 것이다.

우선 마우스 값을 받기 위해, `init()` 구문에서 `glfwSetInputMode` 와 `glfwCursorPosCallback` 을 이용해 마우스 설정과 마우스 입력이 있을 때 호출할 콜백 함수를 설정한다.

``` c++
// Mouse camera control
float g_cur_last_x  = static_cast<float>(SCREEN_WIDTH / 2);
float g_cur_last_y  = static_cast<float>(SCREEN_HEIGHT / 2);
auto g_sensitivity  = 0.05f;
auto g_yaw          = 0.f;
auto g_pitch        = 0.f;

init() {
  	// Create Window...
  	// etc...
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
}

// ...

void mouse_callback(GLFWwindow* const window, double x_pos, double y_pos) {
    float x_offset = x_pos - g_cur_last_x;
    float y_offset = y_pos - g_cur_last_y;

    g_cur_last_x = x_pos;
    g_cur_last_y = y_pos;

    x_offset *= g_sensitivity;
    y_offset *= g_sensitivity;

    g_yaw   += x_offset;
    g_pitch += y_offset;

    if (g_pitch > 89.f)     g_pitch = 89.f;
    if (g_pitch < -89.f)    g_pitch = -89.f;
}
```

### Zoom

줌 역시 Projection matrix 의 `fov` 값을 줄이거나 늘임으로써 줌 인 및 줌 아웃을 할 수 있다.

``` c++
glfwSetScrollCallback(window, scroll_callback); 

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
  	fov -= yoffset;
  if(fov <= 1.0f)
  	fov = 1.0f;
  if(fov >= 45.0f)
  	fov = 45.0f;
}

// ...
projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);  
```

