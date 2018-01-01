# Chapter 21. Instancing

같은 쉐이더와 같은 모델을 이용해 복제본을 엄청나게 많이 만들고 싶을 때, 지금까지라면 다음과 같이 쓸 수 있을 것이다.

``` c++
for(unsigned int i = 0; i < amount_of_models_to_draw; i++) {
    DoSomePreparations(); // bind VAO, bind textures, set uniforms etc.
    glDrawArrays(GL_TRIANGLES, 0, amount_of_vertices);
}
```

하지만 `glDrawArrays` `glDrawElements` 와 같은 함수로 무작정 많은 수의 인스턴스를 그려내고자 할 때는 성능 저하 뿐만 아니라, 하드웨어가 감당할 수 있는 렌더링의 한계를 뛰어넘고도 남을 것이다. 왜냐면 위 두개의 함수는 OpenGL 이 GPU 에게 어느 버퍼를 읽게 해야할 지, VAO 의 버텍스 속성을 어느 것을 읽게 해야할지를 CPU 와 GPU 사이의 버스로 전송하는, *준비과정* 이 있기 때문이다. 이 버스가 꽉 막혀버리면 아무리 GPU 가 좋아도 끝장이 난다.

그래서 일일이 준비과정 하고 렌더링을 하는 것을 몇십만번 할 바에야, GPU 에게 *준비 과정*을 한번만 하도록 시키고, OpenGL 에게 한번의 렌더링 콜 함수로 여러 개의 오브젝트들을 그릴 수 있게 한다면 좋을 것이다. 이것이 바로 **Instancing** 이다.

**Instancing** 은 한번의 렌더링 호출로 많은 수의 오브젝트를 그릴 수 있도록 하는 기법이다. 한번만 오브젝트의 속성 정보와 버퍼 정보를 전달함으로써 CPU 와 GPU 사이의 버스의 병목을 막는다. 이것을 하기 위해서는 `glDrawArraysInstanced` 라는 함수를 이용해 렌더링을 호출한다. (`glDrawElementsInstance` 함수 또한 존재한다)

위의 instancing 함수들은, 여러 개의 오브젝트를 그려야 하기 때문에 추가로 *instance count* 인자를 받는다. 이 함수를 호출하면, GPU로의 버스를 통해 필요한 데이터와, 어떻게 그려야 할지를 한번에 보내기 때문에 병목이 일어날 가능성을 줄인다.

하지만 저 함수 자체로만 호출하는 기에는 여러 개를 만들어도 다 같은 위치에 다 같은 방법으로 렌더링되기 때문에 오로지 한 개로만 보인다. 그래서 여러 오브젝트를 만들어도 제각기 다른 위치나 다른 방법으로 렌더링 될 수 있도록, glsl 에서는 `gl_InstanceID` 라는 built-in 변수를 **Vertex Shader** 에서 제공하고 있다.

``` c++
in int gl_VertexID;
in int gl_InstanceID;
in int gl_DrawID; //Requires GLSL 4.60 or ARB_shader_draw_parameters
in int gl_BaseVertex; //Requires GLSL 4.60 or ARB_shader_draw_parameters
in int gl_BaseInstance; //Requires GLSL 4.60 or ARB_shader_draw_parameters
```

이 값은 Instancing 을 이용해서 렌더링을 호출 시에, 각 인스턴스마다 $$ 0 $$ 부터 올라간다. 만약 43번째 인스턴스에 대해서 뭔가를 하고 싶게 한다면 그 인스턴스의 ID 는 $$ 42 $$ 이다. 이 ID 를 통해 배열을 uniform 으로 가져오거나, 아니면 직접 쉐이더 안에서 위치를 다르게 지정할 수 있다.

이제 이것을 어떻게 구현해서, 한번에 모든 정보를 버스로 이동시킬 수 있는지 알아본다. 첫번째로 구현 가능한 방법은, `uniform` 배열을 이용해서, 배열 전체의 값을 이동시킨 다음에 `gl_InstanceID` 을 사용해 인덱스값으로 값을 찾아 쓰는 방법이 있다. 하지만 이 방법은 **고정형이 아닌 동적인** 인스턴스 값에 대해서는 대처 불가능한 점이다.

따라서 설명할 두번째 방법이 **Instancing** 에서 가장 많이 쓰이는 방법이 된다. 이 방법은 Vertex Shader 에서 layout 지정자를 통해 여러가지 변수를 location 을 지정해서 들여올 때, 인스턴스들의 고유 값을 `VBO` 와 VAO 와의 바인딩을 이용해 입력하는 방법이다. 이 방법은 인스턴스의 개수를 세지는 않지 때문에 동적으로 대처 가능한 장점이 있다.

## Instanced Arrays

Uniform 을 사용해서 모든 배열을 한번에 다 넣어줘야 했던 것과는 다르게, `layout` 지정자를 사용해서 쓸 수 있는 **Instanced Arrays** 는 버텍스 속성으로 지정이 가능하며 새로운 인스턴스(모델) 을 렌더링할 때만 값이 갱신이 되기 때문이다. 버텍스 쉐이더는 현재 처리할 정점에 속해 있는 정점 속성을 가져오도록 GLSL 에게 지시할 수 있다. 하지만, **정점 속성**이 **instanced array** 일 경우에는 정점이 아니라 **인스턴스**의 변경 시점에 한해서만 값의 갱신이 가능하다.

따라서 `layout` 지정자를 사용해도 정점마다 값이 갱신되는 것이 있는가 하면, 인스턴스마다 갱신되는 변수가 동시에 존재할 수 있다.

다음은 Instanced Arrays 을 활용한 사각형을 100개 출력하는 프로그램의 Vertex Shader 프로그램이다.

``` c++
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main() {
    gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    fColor = aColor;
}  
```

이제 쉐이더 프로그램 안에서는 `gl_InstanceID` 을 쓰지는 않는다. 다만 인스턴스마다 `aOffset` 을 갱신하기 위해 `glVertexAttribDivisor` 라는 함수를 쓴다.

``` c++
unsigned int instanceVBO;
glGenBuffers(1, &instanceVBO);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0); 

glEnableVertexAttribArray(2);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
glBindBuffer(GL_ARRAY_BUFFER, 0);	
glVertexAttribDivisor(2, 1);  
```

우선 다른 값들과 마찬가지로 VAO 에 VBO 가 바인딩 되어 있어야 하기 때문에 일련의 과정을 수행해 주고, VAO 에 VBO 의 포인터를 설정해준다. 그리고 `glVertexAttribDivisor` 라는 함수를 이용해 2번 속성을 인스턴스가 갱신 될 때마다 값을 갱신할 수 있도록 설정한다.

이 값은 원래 $$ 0 $$ 으로 설정되어 있는데, 이러면 2번 속성에 대해서 Vertex shader 을 한번 할 때마다 (한 정점마다) 값을 갱신한다는 의미를 지닌다. 이를 숫자를 변경해서 몇 인스턴스마다 값을 갱신할 수 있을지 설정해줄 수 있다.

따라서 출력하면 다음과 같이 결과가 나온다.

![img](21_instanced.png)

물론 `gl_InstanceID` 는 계속 built-in 상태로 남아있기 때문에 이를 이용해 여러가지 효과를 낼 수 있다.

## An asteroid field

Instanced Array 와 Instancing 을 이용해서 태양계의 토성의 소행성단처럼 만드는 것도 가능하다. 다만 이 경우에는 Model 의 `.Draw()` 함수를 사용하기 때문에 Instancing 을 지원하기 위해서 코드 수정이 필요하다.

이태까지 사용된 `.Draw()` 는 `glDrawElements` 을 사용하기 때문에 Instancing 을 지원하지 못한다. 따라서 만약 1000 개 이상의 소행성 모델이 이 함수로 그려진다면 버스 데이터 전송의 병목으로 프레임이 많이 떨어지는 것을 볼 수 있을 것이다. 

다음은 Fragment shader 의 모습이다. `layout` 지정자로 `mat4` 타입이 온 것을 알 수 있다.

``` c++
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0); 
    TexCoords = aTexCoords;
}
```

여기서 주의해야 할 점은, 레이아웃 인자로 `mat4` 등의 매트릭스가 오게 되면, 차지하고 있는 위치 뿐만 아니라 그 매트릭스의 행 수에 해당하는 추가적인 속성 위치가 암시적으로 속성으로 지정된다. 위에서는 location 이 3이기 때문에, 3, 4, 5, 6 이 `instanceMatrix` 의 속성으로 지정된다.

따라서 `glVertexAttribPointer` 을 사용해서 속성과 개수, offset 과 stride 을 지정할 때 다음과 같이 열의 크기와 열의 수 등을 맞춰서 넣어줘야 한다.

``` c++
// vertex Buffer Object
unsigned int buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
  
for(unsigned int i = 0; i < rock.meshes.size(); i++) {
    unsigned int VAO = rock.meshes[i].VAO;
    glBindVertexArray(VAO);
    // vertex Attributes
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
}  
```

그리고 각 메시를 Instancing 하기 위해서 (모델을 다 하나씩 만드는게 아니라, 모델을 구성하고 있는 메시 별로 Instancing Rendering 하는 것 같음) `glDrawElementsInstanced` 를 이용해 그려낸다.

![img](21_debris.png)