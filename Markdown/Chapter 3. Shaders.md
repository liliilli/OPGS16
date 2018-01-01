# Chapter 3. Shaders

* `Shader` 는 GPU 에서 구동되는 작은 프로그램들이다. `Shader` 는 그래픽 파이프라인의 각 프로그래밍 가능한 부분을 메우고 있다.
  `Shader` 는 데이터 독립성을 지원하며 Input 을 Output 으로 바꿔서 다음 단계에 넘겨주는 역할을 한다.

## GLSL

* `Shader` 프로그램은 `GLSL` 이라는 C 와 비슷한 문법을 가진 스크립팅 언어로 구성된다.
  `GLSL`은 여타 다른 프로그램과 동일하게 다음과 같은 구성을 가진다.

> 1. 입력 변수
> 2. 출력 변수
> 3. `Uniform` 변수
> 4. main 함수

다음처럼 쓸 수 있다.

``` c++
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;
  
uniform type uniform_name;
  
void main()
{
  // process input(s) and do some weird graphics stuff
  ...
  // output processed stuff to output variable
  out_variable_name = weird_stuff_we_processed;
}
```

* `Vertex Shader` 프로그램에서 각각의 `Input`은 **Vertex Attribute** 이다.
  `Vertex Attribute`는 하드웨어에 의해서 입력할 수 있는 수가 제한되어 있다. 하지만 적어도 한번에 `vec4` 타입의 속성을 16개 이상 끌어다 쓸 수 있도록 OpenGL 사양에서 지원해주고 있다.

쓰고 있는 하드웨어가 얼마만큼의 `Attribute` 를 지원하고 있는지 알고 싶다면, 다음처럼 코드를 쓰면 된다.

``` c++
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
```

## Types

GLSL 은 C 의 기본적인 타입 외에도 다음과 같은 추가의 타입을 지원한다.

1. `vectors`
2. `matrices`

### Vectors

GLSL 의 `Vector`는 1 개에서 4 개까지의 컴포넌트를 저장할 수 있는 기본 타입이다. 이 타입은 다음과 같은 타입 이름 형식을 가진다.

* `vecn` : the default vector of n floats
* `bvecn` : a vector of n booleans
* `ivecn` : a vector of n integers
* `uvecn` : a vector of n unsigned.
* `dvecn` : a vector of n double components.

거의 모든 컴포넌트는 `vec.x` 과 같은 형태로 접근할 수 있다. 또는 좌표공간의 축 `.x` 을 써서 접근한 것 외에도 색상 공간의 요소 `rgba` 라던가 텍스쳐 공간의 요소 `stpq` 을 사용해서도 동일한 값에 접근이 가능하다.

#### Swizzling

vector datatype 은 다음 예시와 같이 유연하게 벡터 타입의 변수의 값들에 접근해서 사용할 수 있다.

``` c++
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

``` c++
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

### Ins and Outs

각각의 쉐이더 프로그램은 이전 단계나 처음 단계에서 출력물을 입력으로 받아, 재 가공후에 출력을 해야한다. 따라서 이 때 필요한 GLSL 의 키워드가 `in` 과 `out` 이다.

* `Vertex Shader` 에서는 일반 형태의 `in` 키워드를 쓰지 않고 특별한 형태의 `in` 키워드가 들어간 문법을 쓰게 된다. 그렇지 않으면 하나 하나의 버텍스 데이터를 들여오는데 엄청나게 비효율적이게 된다.
* 버텍스 데이터를 Shader 에 들여오기 위해, `(location = 0)`과 같은 문법을 쓴다.
  `(location = 0)` 의 문법을 쓰지 않고도 `glGetAttribLocation` 과 같은 함수로 Attribute 을 가져와 고칠 수 있다. 하지만 대개 `(location = 0)`으로 쓰는 것이 가장 낫다.
* `Fragment Shader` 에서는 컬러 출력으로 `vec4` 포맷을 사용한다. 만약 Fragment Shader 에서 최종 컬러를 출력하는 데 실패하게 된다면 오브젝트 컬러는 검거나 희게만 나올 것이다.
* 각각의 Shader 프로그램에서 출력 변수와 입력 변수의 이름은 같아야 한다.
  쉐이더 오브젝트에서 링킹이 되면서 하나의 통합 프로그램이 되기 때문이다.

### Uniform

* CPU 에서 GPU 로 값을 전달하는 다른 방법이다.
  유니폼 변수는 쉐이더 프로그램의 일반적인 `in` `out` 변수와는 다르게, `global` 변수로 전체 파이프라인에 작용한다.
* 만약 `uniform` 변수를 선언하고 전체 파이프 라인에서 쓰지 않으면, GLSL 컴파일러는 유니폼 변수를 쓰지 않는다고 가정하고 지울 수 있다.
* `uniform` 변수는 코드에서 직접 변수 값을 설정해줘야 한다.
  이를 위해 `glGetUniformLocation` 과 `glUniformXX` 와 같은 함수가 존재한다.

``` c++
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main() {
    FragColor = ourColor;
}   
```

``` c++
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

여기서 알아야 할 점은, `glUseProgram` 함수로 쉐이더 프로그램을 가동시킨 후에, 묘화 함수를 호출하기 사이에 `uniform` 변수의 값을 갱신해야 한다는 점이다.

## More Attributes!

`Vertex Data`로 정점과 관련된 정보 외에도 색상 등과 같이 여러가지 정보를 추가로 보낼 수 있다. 다만 Data 에 추가적인 정보를 같이 보낼 때는 `Stride` 및 `Offset` 등을 잘 조절해야 원하는 GL 프로그램이 동작할 것이다.

``` c++
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};    
```

여기서는 정점의 위치 외에도, 색상의 정보를 같이 보낸다고 하자. 그리고 `Vertex Shader` 의 코드는 다음과 같다.

``` c++
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}   
```

위 GLSL 코드에서, `(location = 1)` 로 컬러의 값을 `vec3` 타입으로 받는 것을 확인할 수 있다. `(location = 1)` attribute 에 버텍스 데이터의 값을 넣어주기 위해서는 `glVertexAttribPointer` 와 `glEnableVertexAttribArray` 함수를 이용해 설정을 해주면 된다.

``` c++
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

![Interleaved data of position and color within VBO to be configured wtih <function id='30'>glVertexAttribPointer</function>](https://learnopengl.com/img/getting-started/vertex_attribute_pointer_interleaved.png)

그리고 `Fragment Shader` 는 `Vertex Shader` 의 `outColor` 변수를 `in`으로 입력하게 해 놓고, vec4 로 변환시켜 픽셀의 최종값을 결정하게 하면 된다.

따라서 다음의 결과를 얻을 수 있다.

![img](https://learnopengl.com/img/getting-started/shaders3.png)

여기서 궁금한 점은, 각 3개의 정점에 대해서만 색상 정보를 넣었을 뿐인데, 실제로는 3색이 서로 혼합이 되서 표시된다는 것이다. 왜냐면 `Fragment Shader`에서 **Fragment Interpolation** 이 일어났기 때문이다. 

* 사실, 래스터라이제이션 단계에서는 우리가 맨 처음에 3개의 정점을 통해서 하나의 Fragment 을 만들 것이라고 생각하는 것과는 다르게, 매우 많은 Fragment 가 생성된다. 그리고 전체 삼각형 모양을 기준으로 각각의 Fragment 의 위치를 정하게 된다.
  각각의 Fragment 의 위치를 `Fragment Shader`의 입력으로 삼아서 **색을 보간**한다.