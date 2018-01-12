# Chapter 17. Cubemaps

**Cubemap** 은 보통 6 개의 독립적인 2D 텍스쳐가 하나의 형태꼴로 묶인 텍스쳐 맵을 말한다. 이 맵에서 각각의 텍스쳐는 하나의 면을 구성한다. 그런데, 사실 각각 따로된 6 개의 텍스쳐를 쓰지 않고 왜 이 큐브 맵을 쓰냐고 하면 큐브 맵은 각각의 면이 방향 벡터를 이용해서 **Indexed** 될 수 있을 뿐만 아니라, **Sampled** 될 수 있기 때문이다.

1x1x1 단위면적 큐브와 그 원점에 방향 벡터의 시작점이 위치한다고 할 때, 큐브 맵의 어느 한 면의 텍스쳐의 값을 구하는 것은 다음과 같다.

![Indexing/Sampling from a cubemap in OpenGL](https://learnopengl.com/img/advanced/cubemaps_sampling.png)

> 방향 벡터가 노멀 벡터여야 하는지는 별로 중요하지 않다. OpenGL 이 방향 벡터가 사이드 면에 지나가는 위치에 존재하는 Texel (텍스쳐의 픽셀) 을 적절하게 샘플링해서 반환해주기 때문이다.

## Creating a cubemap

*Cubemap* 역시 여타 다른 텍스쳐와 같은 텍스쳐의 한 종류이다. 따라서 다음과 같이 텍스쳐를 만들고 Binding 해준다.

``` c++
// Create a cubemap
GLuint texture_cubemap;
glGenTextures(1, &texture_cubemap);
glBindTextures(GL_TEXTURE_CUBE_MAP, texture_cubemap);
```

이제, `glTexImage2D` 을 이용해서 텍스쳐의 정보를 가져온다. 다만 주의해야 할 점은, 큐브맵은 총 6개의 텍스쳐를 받아오는데 각 Texture 가 어디 부분에 타게팅이 되어야 할 지도 알려줘야 한다는 것이다. 즉, 6개의 텍스쳐에 대해서 따로따로 다음의 키워드를 이용해 설정을 한다.

|          Texture target          | Orientation |
| :------------------------------: | :---------: |
| `GL_TEXTURE_CUBE_MAP_POSITIVE_X` |    Right    |
| `GL_TEXTURE_CUBE_MAP_NEGATIVE_X` |    Left     |
| `GL_TEXTURE_CUBE_MAP_POSITIVE_Y` |     Top     |
| `GL_TEXTURE_CUBE_MAP_NEGATIVE_Y` |   Bottom    |
| `GL_TEXTURE_CUBE_MAP_POSITIVE_Z` |    Back     |
| `GL_TEXTURE_CUBE_MAP_NEGATIVE_Z` |    Front    |

다만 GL_TEXTURE_CUBE_MAP... 들은 서로 연속된 값으로 되어 있기 때문에 다음과 같이 `for` 루프를 통해 간단하게 큐브맵의 텍스쳐를 불러올 수 있다.

``` c++
int width, height, nrChannels;
unsigned char *data;  
for(GLuint i = 0; i < textures_faces.size(); i++) {
    data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    );
}
```

그리고, Cubemap 은 다음과 같은 필터링 메소드를 적용하는 게 좋다.

``` c++
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
```

여기서 보이는 `GL_TEXTURE_WRAP_R` 은 texture 의 `R` 축에 대해서 필터링을 하도록 한다. 그리고 `GL_CLAMP_TO_EDGE`을 한 이유는 두 보이는 텍스쳐에 끼인 텍스쳐 축에서는 제대로 face 가 나타나지 않을 수도 있기 때문이다.

그리고 여기서도 `VAO` 및 `VBO`가 필요하다. 큐브맵에 필요한 뼈대는 있어야 하기 때문이다. 아무튼 이런 방법을 사용해서 쓸 수 있는 기법이 *skybox* 이다.

### Skybox

![Image of morrowind with a skybox](https://learnopengl.com/img/advanced/cubemaps_morrowind.jpg)

Skybox 는 여러가지 환경을 둘러싸는 6 개의 이미지로 이루어진 매우 큰 큐브를 말한다. 이 큐브를 씀으로써 위의 이미지처럼 하늘등을 나타낼 수 있다. 스카이박스를 만들기 위한 6개의 이미지 텍스쳐는 다음의 형식을 띌 수 있다.

![Image of a skybox for a cubemap in OpenGL](https://learnopengl.com/img/advanced/cubemaps_skybox.png)

물론 통짜 텍스쳐를 써서 큐브맵의 텍스쳐를 구현하지는 않고, 하나하나마다 따로 따로 존재해서 이것들을 불러와 Cubemap을 만든다.

## Implementing a skybox

1. 큐브맵에 쓰일 VAO 와 VBO 을 만든다.
2. 큐브맵에 쓰일 텍스쳐 6장을 가져와 `GL_TEXTURE_CUBE_MAP_` 인자를 통해 큐브맵 텍스쳐 타입으로 만든다.

``` c++
data = stbi_load(("17_textures/skybox/" + cubemap_paths[i] +
                  ".jpg").c_str(), &width, &height, &channels, 0);
if (data) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                 height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
else {
    std::cout << "Cubemap texture failed to load at path: " << std::endl;
    stbi_image_free(data);
}
```

3. 스카이박스를 만든다. 여기서, OpenGL 에서 뽑아주는 방향 벡터를 통해 3차원 텍스쳐의 텍셀을 파악할 예정이다.
   Vertex Shader와 Fragment Shader 는 다음과 같다. 이 때 gl_Position 의 좌표가 `.xyww` 로 하는 이유는, **Early depth testing** 을 수행하기 위해서이다. early depth testing 은 프래그먼트 쉐이더를 거치기 이전에 발생될 수 있는데, `glDepthFunc` 에 정해진 설정값에 따라서 쉐이더를 거치지 않아도 될 프래그먼트를 폐기할 수 있다. 따라서 성능이 높아진다.

``` c++
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoord = aPos;
    gl_Position = projection * view * vec4(20 * aPos, 1.0);
	gl_Position = gl_Position.xyww;
}  
```

``` c++
#version 330 core
out vec4 FragColor;
in	vec3 TexCoord;

uniform samplerCube cubemap;

void main() {    
	FragColor = texture(cubemap, TexCoord);
}
```

4. 마지막으로 렌더링을 한다. 이 때 위의 Early depth testing 을 제대로 적용하기 위해서 `glDepthFunc(GL_NEQUAL)` 을 쓴다. 그러면 Clip space 의 좌표 값인 `gl_Position` 의 `z` 축 값에 따라서, 현재 Depth buffer 의 값이 없는 부분만 (오브젝트가 그려지지 않은 부분만) 스카이맵이 그려진다.

``` c++
glDepthFunc(GL_LEQUAL);
shader_cube.Use(); {
    shader_cube.SetVecMatirix4f("view", m_view);
    shader_cube.SetVecMatirix4f("projection", camera.GetProjection());

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);
    glBindVertexArray(sky.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
```

## Environment Mapping

큐브맵(스카이맵) 등을 환경 요소로 적용함으로써 그 안에 있는 오브젝트들에게 **반사 요소** 를 적용할 수 있다. 큐브맵을 환경 요소로 써서 뭔갈 하는 테크닉을 **Environment mapping** 이라고 한다. 그리고 이 맵핑 중 가장 자주 쓰이는 기법이 **반사** 와 **굴절** 이다.

### Reflection

반사라는 개념 자체는, 말 그대로 주변 환경을 반사해서 보는 사람의 눈에 정보가 들어가는 것이다. 반사는 물체를 보는 사람의 시점에 따라서 반사되는 주변 환경이 달라진다. 따라서 이 반사를 구현할려면, *카메라의 위치*, *프래그먼트의 노멀 벡터*, *주변 환경의 텍스쳐* 가 필요하다.

![Image of how to calculate reflection.](https://learnopengl.com/img/advanced/cubemaps_reflection_theory.png)

$$ \overline{I} $$ 와 $$ \overline{N} $$ (모두 노멀라이즈 된) 을 구해, 이의 입사각과 반사각을 구해 반사각에 따른 $$ \overline{R} $$ 을 구한다. 여기서 $$ \overline{R} $$ 의 방향 자체가 큐브맵 텍스쳐의 텍셀 위치가 된다.

``` c++
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
```

그리고 반사 구현에 쓰이는 모든 벡터는 World 공간에서 이루어진다. 왜냐면 Camera 공간에서는 모든 벡터가 카메라에 맞춰서 변형되기 때문에 주변 환경의 반사가 제대로 안 이루어진다.

이를 출력한 결과는 다음과 같다.

![img](17_cubemap_reflection.png)

이 기법은 일반 육면체 뿐만 아니라, 모델의 **Reflection map** 과 shader 을 이용해 모델의 일부분만을 반사가 되게끔 할 수도 있다.

### Refraction (굴절)

굴절은 **Snell's law** 을 따른다. 이 역시 정적 모델에서는 굴절되어 비칠 텍스쳐를 입력으로 받아야 한다.

![Image explaining refraction of light for use with cubemaps.](https://learnopengl.com/img/advanced/cubemaps_refraction_theory.png)

굴절 대상이 되는 표면에 치솟은 노멀 벡터가 $$ \overline{N} $$ 이고, 카메라에서 이 표면의 프래그먼트를 보는 방향이 $$ \overline{I} $$ 이며, $$ \overline{R} $$ 이 굴절된 방향이라면, 이 노멀 벡터를 기준으로 $$ \frac{입사각}{굴절각} $$ 이 굴절 계수 (Refractive index)가 된다.

| Material | Refractive index |
| -------- | ---------------- |
| Air      | 1.00             |
| Water    | 1.33             |
| Ice      | 1.309            |
| Glass    | 1.52             |
| Diamond  | 2.42             |

따라서 $$ \overline{R} $$ 이 큐브맵의 굴절 텍셀 위치가 되기 위해서는, 이 계수를 역수로 이용해서 다음과 같이 텍셀의 위치를 계산할 수 있다.

``` c++
void main() {             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}  
```

 따라서 다음과 같이, 굴절된 육면체를 얻을 수 있다.

![img](17_refraction.png)

### Dynamic environment maps

지금까지 해 온 반사 및 굴절 구현은, 실제 복잡하게 오브젝트가 움직이고 여러 오브젝트가 동시에 상호작용으로 반사되는 그런 것은 아니다.

따라서 만약 여러가지 주위의 물체가 해당 물체에 비치게 하고 싶다면, 오브젝트에서 6 방향을 바라본 결과를 텍스쳐로 만들어 (FrameBuffer 을 이용해서) 이를 큐브맵으로 만들어 보다 정확한 반사와 굴절을 해내게 할 수도 있다. 이 방법을 **Dynamic Environment mapping** 이라고 한다. 왜냐면 동적으로 환경을 캐치해내서 텍스쳐를 만들어 큐브맵으로 쓰기 때문이다.

하지만 이 방법은 매우 많은 오브젝트가 있을 때 성능이 매우 요구되는 단점이 있다. 따라서 실제 업계 상에서는 이 동적 환경 맵핑에 여러가지 트릭을 이용해서 쓴다.

![img](17_model.png)

