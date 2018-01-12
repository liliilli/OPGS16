# Chapter 2. Hello Triangle

* OpenGL 에서는 모든 것이 3D 좌표 공간에 있다. 하지만 window 는 2D 화면이기 때문에 OpenGL 은 3D 좌표 공간의 여러가지 요소들을 2D 공간으로 집어넣어야 한다.
* 따라서 3D coordinates (좌표 공간) 을 2D 좌표 공간으로 변환하는 것을 OpenGL 의 **그래픽 파이프라인** 이 관리하게 된다. 그래픽 파이프라인은 크게 두 가지로 나뉘게 된다.
  1. 3D 좌표 공간을 2D 좌표 공간으로 변환하는 단계
  2. 2D 좌표 공간을 실제 모니터의 픽셀들로 변환하는 단계

> 여기서 주의해야 할 점은, 2D 좌표 공간과 픽셀은 완전히 다른 것이라는 것이다.
> 2D 좌표 공간의 요소들은 매우 정확한 위치를 가지고 있는 각각의 요소이지만, 픽셀은 현재 쓰고 있는 모니터나 윈도우의 사이즈에 맞춰서 대략적인 위치로 변환된 요소이다.

* 각각의 그래픽 파이프라인의 단계는 병렬로 처리가 가능한데, 이는 각 단계의 작은 프로그램들을 구동할 수 있는 무수한 코어가 있기 때문이다. ![The OpenGL graphics pipeline with shader stages](https://learnopengl.com/img/getting-started/pipeline.png)
  버텍스 쉐이더나 프래그먼트 쉐이더 및 레즈터라이제이션과 같은 모든 단계는 작은 프로그램으로 각자 맡은 역할을 수행하는데, 이 작은 프로그램들을 **쉐이더**라고 한다.
* 몇몇 단계에서의 쉐이더는 프로그래밍 가능한 형태이며, 몇몇 단계는 고정 함수를 이용해 몇 가지의 모드만 골라 수행할 수 있다. 이런 작은 프로그램인 쉐이더는 *OpenGL Shading Language*(**GLSL**) 라는 언어를 이용해 쓸 수 있다.

## Stages

1. **Vertex Data**
   3D 좌표 공간에서 Triangle 을 형성하는 3개의 점의 위치를 담은 배열들을 일컬어 Vertex Data 라고 한다.
   * **Vertex** 는 3D 좌표 공간에서의 데이터의 모음을 말한다.
     사실 이 Vertex Data 는 *vertex attributes* 라고 하는, 여러가지 데이터들을 담을 수 있다.

>사실 OpenGL 이 Vertex Data 로 결과물을 렌더링하기 위해서는 OpenGL 에게 어떤 방식으로 렌더링을 할 것인지 **타입**을 정해줘야 한다. 따라서 일반 삼각형으로도 렌더링 할 수 있고, 점으로도 가능하며, 아니면 하나의 긴 선으로도 렌더링이 된다.
>이런 타입들은 **Primitive** 라고 하며, OpenGL 이 묘화 처리시의 기준이 된다.
>이 힌트들 중에서는 *GL_POINTS, GL_TRIANGLES, GL_LINE_STRIP* 등이 있다.

2. **Vertex Shader**
   첫 번째로 *attributes* 을 이용해 프로그래밍 가능한 스테이지이다. 이 쉐이더는 단일 Vertex 을 입력으로 받는다. 버텍스 쉐이더의 가장 큰 목적은 이 단일 Vertex 을 일반 3D 좌표 공간에서 다른 3D 좌표 공간으로 변형하는 것이다. 
3. **Primitive Assembly**
   버텍스 쉐이더로부터 변형된 모든 버텍스를 입력으로 받아서 Primitive 설정에 따라 Primitive 을 형성하는 단계이다. 만약 Primitive 가 *GL_POINTS* 라면 일일히 하나의 버텍스씩을 받는다.
4. **Geometry Shader**
   프리미티브 어셈블리에서 각각의 버텍스로 프리미티브가 만들어졌으면, 지모메트리 쉐이더에 입력으로 보낸다. 따라서 지오메트리 쉐이더는 프리미티브를 형성하고 있는 버텍스의 컬렉션을 입력으로 받는다.
   지오메트리 쉐이더는 기존 프리미티브에서 새로운 버텍스를 만들어서 좀 더 디테일하게 프리미티브를 형성시킨다.
   위에서도 알 수 있듯이 지오메트리 쉐이더에서 하나의 삼각형이 두개의 삼각형으로 변형되는 것을 알 수 있다.
5. **Rasterization Stage**
   지오메트리 쉐이더에서 나온, 프리미티브를 형성하는 버텍스들을 최종 스크린에 출력 될 수 있도록 픽셀로 변환하는 작업을 담당한다. 이 픽셀들은 *Fragment* 을 형성한다.
   또한, 스크린 밖에 불필요한 Fragment 을 제거하는 **Clipping** 을 수행한다.

> **Fragment** 는 OpenGL 이 각 픽셀을 렌더링하기 위해 필요한 데이터의 집합을 말한다.

6. **Fragment Shader** 
   프래그먼트 쉐이더는 각 픽셀의 최종 색을 결정하는 단계이다. 이 쉐이더는 텍스쳐의 정보나, 현재 Scene 의 정보 (빛, 그림자, 등) 을 참조로 해서 색을 결정한다. 최종으로 결정된 오브젝트는 마지막으로 **Alpha test**, 혹은 **Blending** Stage 로 넘어가 최적화 및 블렌딩 작업을 담당하게 된다. 이 단계에서 프래그먼트 쉐이더에서 지정한 색과는 다른, 별개의 색이 다시 지정될 수도 있다.

## Vertex Input

OpenGL 을 이용해서 무언가를 그리기 위해선 입력 버텍스 데이터가 필요하다. OpenGL 은 3D 좌표 공간을 기본으로 하고 있기 때문에, 입력 배열 역시 **x, y, z** 을 입력 값으로 하는 배열이 필요하다. 하지만 OpenGL 은 입력으로 넣은 모든 배열을 2D 픽셀의 스크린으로 변환시키지 **않는다.**

* OpenGL 은 x, y, z 축에 대해 `-1.f` 와 `+1.f` 사이의 버텍스 값만을 처리하도록 되어있다.
  이 좌표 공간을 **Normalized Device Coordinates** 라고 한다.

``` c++
float vertices[] = {
    -.5f, -.5f, +0.f,
    +.5f, -.5f, +0.f,
    +0.f, +.5f, +0.f
};
```

> **Normalized Device Coordinates**
>
> ![2D Normalized Device Coordinates as shown in a graph](https://learnopengl.com/img/getting-started/ndc.png)
>
> 버텍스가 버텍스 쉐이더에서 처리되기 위해서는 `-1.f` 와 `+1.f` 사이의 평준화된 좌표 공간 사이에 x, y, z 값이 존재해야 한다. 그 밖에 존재하는 값들은 처리되지 않거나 클리핑 되어 최종 스크린에 보이지 않게 될 것이다. 
>
> 기존 좌표공간이 왼쪽 아래나 왼쪽 위가 원점인 반면에 NDC 에서는 `(0, 0)` 이 중앙이다.
> NDC 좌표 공간은 **glViewport(0, 0, width, height)** 로 받은 묘화 사이즈 데이터를 통해, **viewport transform** 과정을 거쳐 **screen-space coordinates** 좌표 공간으로 바뀌게 된다. 이 과정이 버텍스 쉐이더가 처리하는 과정이다.

GPU 가 일련의 파이프라인 과정을 거치기 위해서는 맨 먼저 버텍스 데이터를 **입력**으로 전달하는 과정이 필요하고, OpenGL 이 어떻게 메모리를 해석하고 그래픽카드에 보내야 할 지 **설정**을 해야한다. 

* **Vertex Buffer Objects (VBO)** 을 통해서 GPU 의 메모리를 저장하거나 관리할 수 있다.
  이 오브젝트를 쓰게 되면 많은 버텍스 데이터를 한꺼번에 그래픽카드에 보낼 수 있으며 이 때 RAM 에서 VRAM 으로 보내는데 필요한 대기 시간을 없애거나 줄여줄 수 있다.
  VBO 는 OpenGL 의 각 오브젝트와 같이, **고유 ID** 을 가질 수 있다. 

``` c++
unsigned int VBO;
glGenBuffers(1, &VBO);
```

* OpenGL 의 **Buffer Object** 는 종류가 많다. 개중에서 VBO 는 **GL_ARRAY_BUFFER** 이다.
  OpenGL 은 다른 버퍼 타입을 가지고 있는 만큼 한번에 버퍼들을 바인딩할 수 있도록 도와준다.

따라서 다음같이 새롭게 만든 버퍼 (아직 ID만 받았고 어떤 버퍼인지는 할당되지 않았음) 를 VBO 의 역할을 할 수 있도록 할 수 있다. 그 후에 버텍스 데이터를 버퍼에 옮기기 위해서 2번 라인과 같이 함수를 실행한다.

``` c++
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

`glBufferData` 의 네번째 인자는 *주어진 데이터를 관리하는 방법* 을 나타내며 다음과 같다.

* GL_STATIC_DRAW
  주어진 데이터는 변하지 않는다.
* GL_DYNAMIC_DRAW
  주어진 데이터가 변할 가능성이 많다.
* GL_STREAM_DRAW
  그려질 때마다 데이터가 변한다.

지금 그리고자 하는 삼각형은 시간이 지나도 변하지 않기 때문에 *GL_STATIC_DRAW* 을 쓰기로 한다. 

* 만약에 버텍스 데이터가 유동적으로 변할 가능성이 있으면 *GL_DYNAMIC_DRAW* 혹은 *GL_STREAM_DRAW* 을 써야한다. 이 모드를 쓰게 되면 GPU 는 DMA 혹은 CPU 에게 수시로 바뀐 데이터를 갱신할 수 있도록 요청할 것이다.

## Vertex Shader

* 버텍스 쉐이더는 파이프라인에서 프로그래밍 가능한 단계 중 하나다.

최근의 OpenGL 은 적어도 버텍스 쉐이더와 프래그먼트 쉐이더를 실장하도록 하고 있다. 따라서 만약에 우리가 어떤 것을 렌더링하고 싶다면 적어도 짧게라도 GLSL 을 이용해 쉐이더를 만들어줘야 한다.

맨 처음에 해야할 작업은, **GLSL 을 이용해서 Vertex Shader** 을 쓰는 것이다.

``` c++
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

* GLSL 은 C 와 거의 닮은 문법을 지원하고 있다.
* GLSL 은 맨 처음에 **버전과 어떤 프로파일을 쓰는 지 명시하는 것**으로 시작해야 한다.

그 후에, 버텍스 데이터들을 버텍스 쉐이더에서 `in` 키워드로 선언한다. 배열의 각 항에 대해서 `aPos` 라는 `vec3` 타입의 '**어떠한 데이터 컨테이너**'로 저장하게 된다.

* GLSL 은 1 에서 4 floats 의 데이터 타입을 가지고 있다.
  위에서는 각각의 버텍스가 3D 좌표공간 상에서의 x, y, z 을 가지고 있기 때문에 우리는 `vec3` 의 컨테이너를 생성해줬다. layout 에 대해서는 후술할 예정이다.

> **Vector**
>
> 그래픽 프로그래밍에서 벡터를 사용하는 이유는, 벡터 자체가 위치와 방향을 동시에 가지고 있으며 수학적 특성이 매우 많이 쓰이기 때문이다.
> vec4 에서는 x, y, z 말고도 `w` 특성이 쓰인다. `w` 특성은 **Perspective Division** 이라고 하는 요소에 쓰이는 값이다.

* `gl_Position` 변수는 *미리 정의된 변수* 이며 vec4 타입을 가진다.
  버텍스 쉐이더의 프로그램이 끝나면 ``gl_Position`` 변수의 값은 결과물로 지오메트리에 쓰이게 될 것이다.

현재는 버텍스 데이터의 값이 NDC 의 조건을 만족하고 있지만, 나중에는 임의의 데이터를 NDC 등으로 변환하는 과정이 필요해진다. (셰이더 프로그램에서 추가적인 작업이 필요해진다는 뜻이다)

## Shader Compiling

버텍스 쉐이더의 프로그램을 C 스트링으로 짜도 되거니와 일반 파일을 써서 짜도 무방하다. 하지만 이 쉐이더 프로그램을 쓰기 위해서는 **런타임에 동적으로 컴파일**을 시켜줘야 한다.

1. 셰이더 오브젝트를 참조하는 ID 변수를 만들어 어떤 셰이더인지를 할당한다.
   `GLuint` 형의 ID 변수는 `glCreateShader` 로 셰이더 타입의 ID을 할당 받을 수 있다.

``` c++
GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
glCompileShader(vertex_shader);
```

* `glCreateShader` 에서 쓰는 쉐이더 타입은 `GL_VERTEX_SHADER`, `GL_GEOMETRY_SHADER`, `GL_FRAGMENT_SHADER` 등이 있다.
* `glShaderSrouce` 함수는 쉐이더 오브젝트를 가져와서 런타임에 컴파일하기 위해 사용된다.
  두번째 인자는 세번째 인자에서 몇 개의 스트링을 가져와 컴파일을 할 것인지 값을 정한다.
  세번째 인자는 쉐이더 프로그램의 스트링을 넣는다.
  마지막은 쉐이더 프로그램의 길이를 제한할 수 있는데, `nullptr` 등으로 `\0`이 될 때까지 읽게 할 수 있다.

> `glGetShaderiv` 와 `glGetShaderInfoLog` 함수를 이용하면, 해당 쉐이더가 컴파일 될 때 오류를 확인할 수 있다.

## Fragment Shader

* Geometry Shader 는 제외하고, Fragment Shader 는 프로그래밍 가능한 단계 중 마지막 단계에 속한다.
* Fragment Shader 는 프래그먼트의 **각 픽셀의 최종 색**을 결정한다.

``` c++
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```

Fragment Shader 는 오직 `vec4` 타입의 변수 하나만 결과물로 요구한다. 따라서 출력할 결과물의 변수를 `out` 키워드로 선언해서 이 결과물이 쉐이더 프로그램에서 출력됨을 나타낼 수 있다.
여기서 vec4 의 값은 `R`, `G`, `B`, `Alpha` 값이다. 

2. 쉐이더를 만들어서 컴파일하는 방법은 위에서 버텍스 쉐이더를 만들어 컴파일 했을 때와 같다.

## Shader Program

* 쉐이더 프로그램 객체는 여러가지 쉐이더들이 결합되어 하나의 통합된 파이프라인으로 동작할 수 있는 프로그램 객체다.
  각각의 쉐이더 프로그램을 쓸려면 각 프로그램을 하나의 쉐이더 프로그램 객체로 링크한 후에, **프로그램 객체를 렌더링할 때 호출한다**.
* 이 프로그램은 각각의 쉐이더의 *input* 과 *output* 을 서로 연결해 주기 때문에 만약에 변수 이름이 맞지 않다거나 하는 경우에는 *linking errors* 가 발생할 수 있다.

3. 쉐이더 프로그램 객체를 만들어서 각 쉐이더를 링크한다.
   이 때 `glAttachShader(program, shader)` 와 `glLinkProgram(program)` 이 쓰인다.
   `glLinkProgram` 은 쓰고자 하는 쉐이더들을 연결시켜서 링킹하는 역할을 맡고 있다.

``` c++
GLuint shader_program = glCreateProgram();
glAttachShader(shader_program, vertex_shader);
glAttachShader(shader_program, fragment_shader);
glLinkProgram(shader_program);
```

4. `glDeleteShader(shader)` 로 링킹한 각각 쉐이더들을 삭제한다.
5. `glUseProgram(program)` 을 호출해서 쉐이더를 작동시킨다.
   이 함수 이후로 여러가지 렌더링 구문들이 호출된다.

하지만 OpenGL 은 아직까지 Vertex Data 를 어떻게 해석해야 할지, 그리고 Vertex Data 을 Attributes 에 연결하는 방법을 모르기 때문에 연결을 해주는 코드를 짜서 넣어줘야만 한다.

##  Vertex Attributes 을 연결하기

* Vertex Data 을 가져오는 Vertex Shader 는 프로그래머블 하기 때문에 매우 유동적으로 다양한 값들을 가져와서 처리할 수 있다. 하지만 역으로 생각해보면 OpenGL 이 Data 을 처리하기 위해서는 어느 부분에 어느 값이 들어와 있는지 알지 않으면 안된다.
  따라서 프로그래머가 **Input data 중에 어느 부분이 어떤 Attribute 에 속하는지를 명시**해야 한다.

다음은 지금 삼각형 Vertex Data 가 Shader 에 입력 됬을때 명시되야 할 데이터 구조다.

![Vertex attribte pointer setup of OpenGL VBO](https://learnopengl.com/img/getting-started/vertex_attribute_pointer.png)

> 각 데이터는 `float` 타입이므로 `sizeof(float) * 8` 비트만큼을 가진다.
> 각 정점은 3개의 축 위치를 가진다.
> 세 개의 값 사이에 빈 공간이 없이, 연속적으로 값이 있다. (*tightly packed*)
> 데이터의 첫 값은 버퍼의 시작이다.

위의 4개의 정보로 OpenGL 에게 한 Attribute 마다의 Data 을 어떻게 해석할 지, `glVertexAttribPointer` 로 알려줄 수 있다.

``` c++
glVertexAttribPointer(0, 3, 
                      GL_FLOAT, GL_FALSE, 
                      3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

* 첫번째 인자
  프로그래머가 설정하고자 하는 ***Vertex Attribute* 의 값**
* 두번째 인자
  Vertex Attribute 의 사이즈로, Byte 가 아닌, **저장할 값의 개수**를 정한다.
* 세번째 인자
  각 Data 의 **타입** 을 정한다.
* 네번째 인자
  현재 읽어들이는 Vertex Data 의 값들을 NDC 로 평준화 할지 선언한다.
  *GL_TRUE* 와 *GL_FALSE* 가 있다.
* 다섯번째 인자
  **Stride** 을 설정한다. Stride 는 연속적인 Vertex Attribute Sets 사이의 공간을 의미한다. 여기서는 각 set 마다 3 개의 `float` 타입의 값이 있기 때문에 `3 * sizeof(float)` 이 Stride 의 값이 된다.
* 여섯번째 인자
  **Offset** 으로, 버퍼에서 데이터가 시작하는 위치를 나타낸다.

> `glVertexAttribPointer` 을 호출할 때, 각각의 Attribute 는 현재 호출된 시점에서 `GL_ARRAY_BUFFER` 에 연결된 VBO 가 관리하는 Vertex Data 에서 데이터를 꺼내온다.
> 따라서 `glVertexAttribPointer` 가 **호출되고 난 뒤에야** VBO 의 데이터가 실제로 렌더링 될 Attribute 에 연결된다.

그 후에, `glEnableVertexAttribArray` 함수로 vertex attribute 의 주소를 설정해준 뒤에 비로소 attribute 을 쓸 수 있게 된다. (그래서 vertex attribute 는 기본적으로 DISABLED 상태다)

간략히 단계를 논하면 다음과 같을 것이다.

1. Vertex Data 을 만들고, VBO 을 셋업해서 안에다가 집어넣는다.
2. 버텍스 쉐이더와 프래그먼트 쉐이더를 컴파일시킨 후에 쉐이더 오브젝트와 링크해서 일종의 파이프라인을 만든다.
3. OpenGL 에게 vertex attributes 에 어떤 Data 가 오고 해석해야 하는지 명시적으로 알린다.
4. ???
5. Profit!

별 다른 방법이 없으면 이 짓을 *각 vertex attribute* 와 *각 object* 에 대해 반복적으로 수행해야 렌더링이 *겨우* 된다. Boilerplate 한 작업을 막기 위해, **현재 렌더링 된 Attribute의 상태 등을 저장하고 불러올 수 있는 오브젝트** 가 사용될 수 있다.

## Vertex Array Object (VAO)

![Image of how a VAO (Vertex Array Object) operates and what it stores in OpenGL](https://learnopengl.com/img/getting-started/vertex_array_objects.png)

* VAO 는 연속된 vertex attribute 와 VBO 와 같이 바인드 될 수 있다.
  vertex attribute pointer 을 설정할 때 한번 VAO 의 호출을 함으로써, 해당 오브젝트를 그릴려고 할 때마다 적절한 VAO 을 다시 바인드 하는 것으로 **쉽게 오브젝트를 렌더링**할 수 있다.
* VAO 는 서로 각기 다른 VAO 를 바인딩하는 것으로 다른 버텍스 데이터와 애트리뷰트 설정을 전환할 수 있다는 것도 장점으로 꼽힌다.

> Core OpenGL 프로파일은 VAO 을 바인딩해서 쓰는 것을 강력히 권장하고 있다.

VAO 의 저장 루틴은 다음과 같다.

1. `glEnableVertexAttribArray` 또는 `glDisableVertexAttribArray` 을 호출한다.
2. `glVertexAttribPointer` 을 통해서 버텍스 속성을 설정한다.
3. `glVertexAttribPointer`로 VBO 가 버텍스 속성에 적절하게 연결된다.

### 단계

1. VAO 를 설정한다. VBO 와 거의 흡사하다.

``` c++
GLuint VAO;
glGenVertexArrays(1, &VAO);
```

* 이 때 `glBindVertexArray` 을 사용해서 **먼저 VAO 을 bind** 시켜줘야 한다. 
  이 후에, 적절한 VBO 나 버텍스 속성 포인터들을 바인드 하거나 설정해야 한다. VBO, EBO, `glVertexAttribPointer` `Enable` 에 관련된 함수 호출 및 설정이 VAO 에 자동으로 바인드 되기 때문이다.

2. 쉐이더 프로그램을 호출한 후, 구체적으로 묘화 함수를 호출하기 전에 **VAO 을 바인드시킨다**.
3. VAO 에 저장된 버텍스 속성과 관련된 VBO 에 따라서 쉽게 렌더링이 되어 스크린에 묘화된다.

* 만약 다수의 오브젝트를 한번에 그리고 싶으면, 한 오브젝트마다 하나 이상의 VAO 을 설정해서 관련된 VBO 와 포인터를 저장할 수 있도록 하면 된다.

4. 묘화가 된 후에는 VAO 을 **unbind** 한다. (자동으로 unbind 된다)

### 결과

루프에서 다음의 코드를 써넣고 돌리면 이하 이미지와 같이 삼각형이 렌더링 되서 출력된다.

``` c++
glUseProgram(shader_program);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

![An image of a basic triangle rendered in modern OpenGL](https://learnopengl.com/img/getting-started/hellotriangle.png)

---

## Element Buffer Object (EBO)

만약에 프로그래머가 사각형 텍스쳐 ( 삼각형 두개로 이뤄진 ) 를 그린다고 하자. 그러면 대충 무식한 방법은 다음과 같을 것이다.

``` c++
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
```

이렇게 짠 다음에, VBO 와 포인터를 만들어 VAO 의 두 포인터에 VBO 의 해당 영역을 참조해서 삼각형 두개를 렌더링한 뒤, 사각형으로 표현하면 될 것 같다. 하지만 이 방법은 무지막지한 버텍스가 렌더링되는 상황에서는 스마트하게 짠 것보다 50% 의 오버헤드가 더 발생되는 참사를 낳는다.

이렇게 하는 대신에 4개의 버텍스를 VBO 에 집어넣게 한 뒤에, **어느 버텍스부터 순서대로 그리도록 할까**만 배열로 지정해줘서 렌더링시키면 위와 같이 더럽게 짤 필요가 없을 것이다. 이것을 도와주는 객체가 **EBO** 다.

* **Element Buffer Object**
  VBO 와 비슷한 버퍼이지만, OpenGL 에게 버텍스 중 어느 것을 먼저 그리게 할 지 정하는 인덱스 배열 (값) 들을 저장할 수 있다. 
  인덱스를 이용해 버텍스를 알뜰하게 그릴 수 있는 방법을 **Indexed Drawing** 이라고 한다.

위의 코드를 다음과 같이 쓸 수 있다.

``` c++
float rectangle_vertices[] = {	// NDCoordinate
    +.5f, +.5f, +.0f,
    +.5f, -.5f, +.0f,
    -.5f, -.5f, +.0f,
    -.5f, +.5f, +.0f
};
unsigned rectangle_indices[] = {
    0, 1, 3, 	// First Triangle
    1, 2, 3		// Second Triangle
};
```

### 단계

1. EBO 을 생성한다. VBO 와 같은 방식으로 만들 수 있다.
2. EBO 를 바인딩 (실체화)한다. 여기서 EBO 는 인덱스 배열을 저장하기 때문에 `GL_ELEMENT_ARRAY_BUFFER` 을 type 으로 써야한다.

``` c++
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices),
             rectangle_indices, GL_STATIC_DRAW);
```

3. EBO 을 이용해서 렌더링을 할 때는
   `glDrawElements(primitive, count, type, indices_offset)` 을 써서 렌더링한다.
   세번째 인자의 *type* 은, `indices_array` 의 값의 타입을 나타낸다.

![Image of VAO's structure / what it stores now also with EBO bindings.](https://learnopengl.com/img/getting-started/vertex_array_objects_ebo.png)

![A rectangle drawn using indexed rendering in OpenGL](https://learnopengl.com/img/getting-started/hellotriangle2.png)

