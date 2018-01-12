# Chapter 4. Textures

보통 상황에서는 수 많은 컬러들이 한 프리미티브 안에 있어야 한다. 이 떄 여태까지 써왔던 방식으로는 엄청나게 많은 버텍스가 필요할 것이며 부수적으로 프래그먼트도 무수하게 생길 것이다. 이는 곧 성능 저하로 직결된다.

여기서 효과적으로 디테일한 색들을 정해 정보를 표현할 수 있는 것이 **Texture** 을 쓰는 것이다. 텍스쳐는 1D로도 3D 로도 존재하지만 대개 2D로 존재하며, 오브젝트에 세부 사항을 넣는데 사용된다. 

이 텍스쳐가 어떻게 사용 되는지만 잠시만 예를 살펴보면, 3D 좌표 공간에 버텍스로 뼈대만 있는 집이 있다고 하자. 그러면 집의 지붕 프래그먼트나 벽 프래그먼트에 잘 만들어진 텍스쳐를 적용하면 여러가지 디테일을 구현하기 위해서 추가적인 버텍스를 만들 필요없이 그럴싸한 디테일을 가진 '색이 입혀진' 집이 완성된다.

> 텍스쳐는 이미지를 저장하는 용도로 쓰이는 것 외에도, 셰이더에 보내기 위한 많은 양의 데이터를 저장하는 저장소의 용도로도 쓰일 수 있다.

![img](https://learnopengl.com/img/getting-started/tex_coords.png)

* **Texture** 는 **Texture coordinate** (텍스쳐 좌표)를 가지고 있다. 이 좌표를 이용해서 어느 부분을 프래그먼트에 가져오게 할 지 결정할 수 있다. 그 후에, Fragment interpolation 가 텍스쳐가 적용되지 않은 다른 프래그먼트에 적용된다.
* `x` 와 `y` 축이 있고, 0 에서 1까지의 범위를 가진다. 최하단 왼쪽이 `(0, 0)` 이다.
  Texture coordinate 을 이용해서 특정 부분에 텍스쳐의 특정 부분의 색을 가져오는 것을 **Sampling** 이라고 부른다.
* 텍스쳐를 가져와서 적용하는 것 역시, `Vertex Data` 와 마찬가지로 각각의 정점마다 텍스쳐의 어느 부분을 가져와 적용할 것인지 알려줘야 한다. 만약 위로 향한 삼각형에 텍스쳐를 적용하고 싶으면 다음과 같이 Data 을 만들어 입력으로 넣어야 한다.

``` c++
float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};
```

* Texture 는 **느슨한 해석 (어떻게 처리할 것인지 귀띔하는 것)** 을 가지고 있다. 따라서 OpenGL 에게 Texture 을 가지고 어떻게 샘플링할 것인지 명시적으로 알려줘야 한다.

## Texture Wrapping

Texture coordinate 는 `(0, 0)` 에서 `(1, 1)`까지를 가진다. 이 때 이 범위 밖의 좌표의 텍스쳐를 가져오고 싶다면 어떻게 해야할까? 기본적으로 OpenGL 은 텍스쳐 이미지를 반복해서 출력한다. 하지만 기본적인 동작 외에 여러가지 옵션들이 있다.

![img](https://learnopengl.com/img/getting-started/texture_wrapping.png)

* `GL_REPEAT` : 텍스쳐 기본 동작. 텍스쳐 이미지를 반복해서 출력한다.
* `GL_MIRRORED_REPEAT` : `GL_REPEAT` 와 같지만, 범위 밖 각 반복마다 이미지를 좌우 또는 상하로 뒤집어서 보여준다.
* `GL_CLAMP_TO_EDGE` : 좌표를 `(0, 0)` 과 `(1, 1)` 사이로 고정한다. 만약 이 밖을 벗어나게 되면 *가장자리* 패턴을 쭈욱 늘려서 출력한다.
* `GL_CLAMP_TO_BORDER` : 프로그래머가 설정한 가장자리 색으로 `GL_CLAMP_TO_EDGE` 와 동일한 행동을 한다.

앞서 말한 옵션들은, 각 축마다 옵션을 다르게 해서 설정할 수도 있다. 쓰이는 함수는 `glTexParameteri` 이다. 이 함수는 텍스쳐 옵션 패러미터를 설정한다.
이 때 Texture 의 축의 이름은 `s`, `t`, `r` 로 부르며 일반 좌표 공간의 `x`, `y`, `z` 와 같다. 다만 2D 에서는 `s` 와 `t` 만 쓰인다.

``` c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

1. 첫번째 인자
   **텍스쳐 타겟**을 설정한다. 2D 텍스쳐의 옵션을 변경하고 싶다면 `GL_TEXTURE_2D`을, 아니면 각자 텍스쳐 공간에 맞는 타입을 설정하면 된다.
2. 두번째 인자
   현재 설정하는 텍스쳐의 **어떤 축** 에 대해 옵션을 갱신하고 싶은지 알린다. `GL_TEXUTRE_WRAP_S` 는 `s` 축, `_WRAP_T` 는 `t` 축에 대해 옵션을 변경하겠다고 알리는 것이다.
3. 세번째 인자
   **Texture Wrapping Mode**을 설정한다. 위의 `GL_` 로 시작하는 모드 중 하나를 택하거나 하면 된다.

## Texture Filtering

* 텍스쳐 자체는 해상도에 구애를 받지 않으며, 각 점이 `float` 타입의 값으로 이뤄진다. 
  따라서 OpenGL 은 `(0, 0)` 에서 `(1, 1)`까지의 텍스쳐 좌표를 어느 텍스쳐 픽셀 (**Texel**) 로 맵핑을 해야할지 알아내야 한다. 

이 과정은 매우 큰 오브젝트에 매우 작은 텍스쳐를 입히고자 할 때 엄청 중요해진다. 만약에 엄청 큰 오브젝트에 텍스쳐 `(0, 0)` 에서 `(1, 1)`을 입힐려고 하면, 오브젝트 중간 중간의 프래그먼트는 텍스쳐 좌표 `(x, y)` 가 매우 정밀한 소수점을 가지게 된다. 이 좌표를 통해서 텍스쳐에서 어떤 색을 뽑아야 하는지 OpenGL 이 알아야 된다.

* OpenGL 은 **Texture Filtering 을 위한 옵션**을 제공하고 있다. 몇 가지가 존재하지만 가장 많이 쓰이는 옵션은 다음과 같다.

![img](https://learnopengl.com/img/getting-started/filter_nearest.png)

1. `GL_NEAREST` (nearest neighbor filtering) : OpenGL 의 가장 기본적인 텍스쳐 필터링 방법이다. 각 텍셀의 *중심점에서 가장 정확한 색*을 골라 샘플링을 한다.

![img](https://learnopengl.com/img/getting-started/filter_linear.png)

2. `GL_LINEAR` (bilinear filtering) : 텍셀 사이들의 색을 잘 추측해서 이웃 텍셀과 보간된 색을 얻는다. 따라서 위와 같이 이웃 텍셀과 혼합된 색을 얻는다.

두 개의 옵션들을 따로 적용한 결과는 밑과 같다.

![img](https://learnopengl.com/img/getting-started/texture_filtering.png)

* 또한 텍스쳐 필터링은 텍스쳐를 스케일 업 **magnify** 하거나 다운 **minify** 할 때에도 각기 다른 옵션을 쓸 수 있도록 할 수 있다. 이 때도 `glTexParamateri` 함수를 써서 옵션을 설정한다.

``` c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

### Mipmaps

만약에 매우 큰 방이 있고, 거기에 카메라를 통해 보이는, 같은 텍스쳐를 가지는 무수한 오브젝트가 거리를 두고 있다고 보자. 그러면 일반 plain 한 상태에서는 아주 멀리 보이는 자그마한 오브젝트나 가깝게 보이는 큰 오브젝트는 동일한 텍스쳐 사이즈를 가질 것이다. 

이 경우 텍스쳐 사이즈가 매우 커지거나 하면 메모리 사용량이 현저히 높아질 뿐만 아니라, 작은 물체의 경우에는 텍스쳐 필터링에 매우 큰 어려움을 겪을 것이다. 따라서 이 단점을 해결하기 위해 나온 것이 **Mipmaps** 이다.

![img](https://learnopengl.com/img/getting-started/mipmaps.png)

* **Mipmaps** 는 기존 텍스쳐 이미지에서 2배씩 축소된 텍스쳐 이미지들의 컬렉션이다.
  밈맵이 쓰이게 되는 기준은 간단하다. 카메라에서 오브젝트 까지의 거리가 일정 수준 이상이거나 이하이면, OpenGL 은 *거리에 맞는 적절한 밉맵 텍스쳐* 를 쓰도록 한다. 
* Mipmaps 는 텍스쳐 필터링과 더불어 메모리 사용량을 줄여주는 등의 성능 저하를 막을 수 있다.

밉맵을 만드는 과정은, 텍스쳐를 생성하고 난 뒤에 `glGenerateMipmaps`을 쓰면 된다. 
카메라 거리에 따라서 OpenGL 이 오브젝트의 밉맵을 바꿀 때, 두 밉맵 레이어 사이에 약간의 튀는 요소(?) 가 생길 수도 있다. 밑의 이미지와 같이 버텍스 사이의 날카로운 선이 생길 수도 있다.

![https://i.imgur.com/QdzWG.png](https://i.imgur.com/QdzWG.png)

이런 부작용을 막기 위해서, 일반 텍스쳐 필터링과 동일하게 `NEAREST` 혹은 `LINEAR` 등과 같은 필터링 옵션을 적용하는 것이 가능하다. 이 역시 `glTexParameteri` 으로 설정이 가능하다.

1. `GL_NEAREST_MIPMAP_NEAREST` : 픽셀 사이즈와 가장 가까운 밉맵을 가져와서, 텍스쳐 샘플링에 `NEAREST` 보간을 적용한다.
2. `GL_LINEAR_MIPMAP_NEAREST` : 픽셀 사이즈와 가장 가까운 밉맵을 가져와서, `LINEAR` 보간을 적용한다.
3. `GL_NEAREST_MIPMAP_LINEAR` : 픽셀 사이즈에 가장 가까운 밉맵 2장을 선형 보간한 뒤에, `NEAREST` 보간을 적용한다.
4. `GL_LINEAR_MIPMAP_LINEAR`: 픽셀 사이즈에 가장 가까운 밉맵 2장을 선형 보간한 뒤에, `LINEAR` 보간을 적용한다.

``` c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

> 이 때 주의해야 할 점은, 밉맵 옵션을 GL_TEXTURE_MAG_FILTER 와 같이 쓰지 말아야 한다는 점이다. 밉맵은 텍스쳐가 축소 될 때 사용되는 기법이기 때문이다. 만약에 옵션으로 넣는다고 해도 확대 될 때는 영향이 전혀 없다.
>
> 또한 MAG_FILTER 에 밉맵 옵션을 넣게 되면 OpenGL 은 `GL_INVALID_ENUM` 에러를 띄울 것이다.

## Loading and creating & generating textures

우선 파일 시스템의 이미지를 데이터로 불러오기 위해서 `stb_image.h` 헤더 라이브러리 파일을 쓰기로 했다. 이 헤더 파일을 통해 `jpg` 혹은 `png` 파일 등을 쉽게 불러올 수 있다.

`stb_image.h` 파일을 이용해서 일반 `jpg` 파일을 `unsigned char*` 포인터 (배열)로 바꾸는 과정은 생략한다.

## Generating Textures

OpenGL 의 모든 오브젝트가 `GLuint` ID 을 갖듯이, 텍스쳐도 해당 타입의 ID 를 가져야 한다.

``` c++
unsigned int texture; 	// GLuint texture;
glGenTextures(1, &texture);
```

* `glGenTextures(GLsizei n, GLuint * textures)`
  `glGenTextures` 함수는 잠재적으로 텍스쳐를 받을 배열과 크기를 받아서, 참조 ID 를 돌려준다.

그 후에 `Binding` 을 시키고, *2D 텍스쳐*를 만든다. `Mipmap` 역시 만들어준다.

``` c++
auto data = ... // Using library, let image be useable data array.
if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
else {
    std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
}
```

* `glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLvoid* data)`

  첫 번째 인자는 위에서 `GL_TEXTURE_2D` 로 바인딩한 텍스쳐 오브젝트에 한해서만 텍스쳐 적용 타겟을 정하겠다는 의미다.

  두 번째는 Mipmap 레벨을 임의로 정하되, 여기서는 기본 레벨로 두겠다는 의미다.

  세 번째는 OpenGL 에게 이미지의 방식에 대해서 어떤 방식으로 텍스쳐를 저장하겠는가를 의미한다. `jpg` 경우에는 `alpha`가 없고, 오로지 `RGB`로만 이뤄져 있기 때문에 `GL_RGB` 로도 충분하다.

  그리고 해당 이미지의 크기가 네, 다섯 번째 인자로 주어진다. 여섯 번째 인자는 `0` 이 되야한다.

  마지막 인자를 뺀 앞의 두 인자는, 소스 이미지의 포맷과 데이터 타입을 적는다.

이 함수가 실행되면, 바로 위에서 Binding 된 `texture` 텍스쳐 오브젝트에 텍스쳐가 갱신된다. 그런데, 지금 `glTexImage2D` 만 호출한 상황이라면, 거리가 달라져도 텍스쳐는 바뀌지 않는다. 왜냐하면 두 번째 인자가 `base_level (0)` 으로 *밉맵이 아직은 생성되지 않았기 때문*이다.

만약에 밉맵 레벨에 따라서 각기 다른 텍스쳐를 넣을 예정이 아니라면, `glGenerateMipmap` 과 같이 자동으로 바인딩된 오브젝트에 설정된 텍스쳐의 밉맵을 만들어 준다.

``` c++
glGenerateMipmap(GL_TEXTURE_2D);
```

따라서 전체적인 텍스쳐 생성 및 설정 코드는 다음과 같다.

``` c++
// Generating a texture
GLuint texture;
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image file to use as texture image.
    auto width = 0;
    auto height = 0;
    auto nr_channels = 0;
    auto data = stbi_load("heroes.jpg", 
                          &width, &height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
    }
    stbi_image_free(data);
}
```

## Applying Textures

버텍스 데이터에 한꺼번에 각 정점의 **위치**, **색상**, **텍스쳐 좌표** 를 넣어 프래그먼트에 텍스쳐를 적용해 나타낼 것이다. 이 때 `EBO` 을 사용해 인덱스에 따라 사각형이 표시될 수 있도록 한다.

``` c++
// Data Setting
float vertex_data[] = {
    // Positions              // Colors               // Texture Coords
    -.75f,   -.75f,   +.0f,   1.0f,   0.0f,   0.0f,   0.0f,   0.0f,
    +.75f,   -.75f,   +.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,
    +.75f,   +.75f,   +.0f,   0.0f,   0.0f,   1.0f,   1.0f,   1.0f,
    -.75f,   +.75f,   +.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f
};

unsigned indices_data[] = {
    0, 1, 3,
    1, 2, 3
};
```

각 정점에 대해 차지하는 공간이 32 Bytes 이기 때문에, `stride` 는 자연스레 32 Bytes 로 결정된다.

``` c++
auto stride = 8 * sizeof(float);
// Positions
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
glEnableVertexAttribArray(0);
// Colors
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
// Texture Coords
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(2);
```

Vertex Shader 와 Fragment Shader 의 코드는 다음과 같다.

``` c++
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
```

``` c++
#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
```

여기서 Fragment Shader 에서 주의해야 할 점은, GLSL 이 `sampler` 라고 부르는 built-in 데이터 타입을 통해 텍스쳐를 부를 수 있다는 점이다. 
그리고 텍스쳐를 샘플링 하기 위해서 `texture` 라는 GLSL 의 built-in 함수를 불러 **텍스쳐**와 **Coords** 을 넣음으로써 출력될 프래그먼트에 적절한 텍스쳐가 입혀져 보여지게 된다.

이제 `ourTexture`에 텍스쳐를 바인딩 하기 위해, `glDrawElement` 와 같은 세부 묘화 함수가 호출되기 이전 부분에 `glBindTexture` 함수를 호출한다. `glBindTexture` 로 Fragment Shader 의 `ourTexture`에 텍스쳐가 자동으로 할당된다.

만약에 텍스쳐 위에 뭔가의 색을 섞고 싶다면, Fragment Shader 프로그램의 코드에 다음과 같이 `*` 연산자를 쓰면 된다.

``` c++
FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
```

## Texture Units

위 코드에서는 Texture 에 아무것도 정하지 않았는데 텍스쳐가 반영된 결과가 출력되는 것을 볼 수 있었다. 그러면 이렇게 되는 이유는 과연 뭘까? 답은 **Texture Unit** 이라는 것이 자동으로 반영되었기 때문이다.

* **Texture Unit**
  Shader 프로그램에 반영되는 각 Texture 의 location 을 말한다. (Vertex data 의 location = n 과 같은 역할을 한다)
  각 Texture Sampler (*Texture2D* 등) 에 `glUniform1i` 등의 함수로 일련의 숫자를 할당하면 바인딩된 텍스쳐의 ID 에 따라 지정된 Texture Sampler 에 바인딩이 된다.

  기본으로 Texture Sampler 는 `0` 의 location 값을 가진다. 따라서 여러 개의 텍스쳐를 한 셰이더에 쓰고 싶으면, `glGetUniformLocation` 과 `glUniform1i` 의 함수를 써서 `uniform` 으로 선언된 텍스쳐 샘플러의 위치 값을 지정해야 한다.

따라서 셰이더에 여러 개의 텍스쳐 샘플러가 있을 때, 묘화 전에 다음과 같이 텍스쳐를 활성화시켜줘야 한다.

``` c++
glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
glBindTexture(GL_TEXTURE_2D, texture);
```

> `GL_TEXTUREn` 매크로는 텍스쳐 샘플러에 바인딩시킬 텍스쳐를 할당하는데 쓰인다. `GL_TEXTUREn` 매크로는 OpenGL 사양으로 최소 16개의 ( 0 ~ 15 개 ) 텍스쳐 샘플러를 지원한다.

Fragment Shader 가 다음과 같고

``` c++
#version 330 core
...

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
```

텍스쳐 2개를 이용해 정사각형의 프래그먼트에 렌더링을 하고자 하면, 다음과 같이 쓰면 된다.

``` c++
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_2);
}

shader_program->Use();
{
    glUniform1i(glGetUniformLocation(shader_program->getId(), "texture1"), 0);
    shader_program->SetInt("texture2", 1);

    auto min_density = glGetUniformLocation(shader_program->getId(), "min_density");
    glUniform1f(min_density, static_cast<float>(g_min_density));
}

glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

* 주의해야 할 점
  OpenGL 은 $$ (0, 0) $$ 좌표를 좌하단으로 인식하나, 이미지는 좌상단으로 인식한다. 따라서 의도치 않게 텍스쳐가 상하로 뒤집혀서 보일 수 있다. 이것을 막기 위해 다음과 같이 이미지를 불러올 때 상하 반전이 되게 불러온다.

``` c++
stbi_set_flip_vertically_on_load(true);  
```

그러면 다음과 같은 결과를 얻을 수 있다.

![img](https://learnopengl.com/img/getting-started/textures_combined2.png)

