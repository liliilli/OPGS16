# Chapter 26. Point Shadows

지난 튜토리얼에서는 Orthographic Projection Matrix 와 Directional Light 을 잘 이용해서 직사광선 그림자를 구현했다. 사실 이 그림자 구현 기법은 **Directional Shadow Mapping** 이라고 알려져 있다. 이번 튜토리얼에서는 한 방향만 바라보는 빛이 아니라 모든 방향에서 그림자를 생성할 수 있도록 할 예정이다. (라고 한다.) 이 기법은 **Omnidirectional Shadow Mapping** 이라고 하는데, Point light 와 spotlight 에 적용될 수 있다.

Omnidirectional Shadow mapping 에 쓰이는 알고리즘 자체는 거의 Directional Shadow mapping 과 거의 같다고 한다.

1. 사용자 정의 프레임버퍼를 만들어서 거기에 Off-screen rendering 으로 Depth buffer 을 만든다.
2. 임의 프래그먼트 지점을 $$ \mathbf{T} $$ 로 옮겨서 Depth map 의 값과 비교한다.
3. 그림자인지 아닌지를 가려낸다.

여기서 두 기법의 차이점은 **Depth map 을 무엇을 쓸 것인가** 이다. 그러면 Omni 의 경우에는 전방의 Depth map 을 가져와야 하는데 어떤 방식으로 할 것인가? 답은 이전 튜토리얼에서 썼던 **Cube map** 을 이용하면 된다. 큐브 맵과 깊이 맵이 합쳐서, **Depth cubemap** 을 프레임 버퍼에서 이용한다.

![Image of how omnidrectional shadow mapping or point shadows work](https://learnopengl.com/img/advanced-lighting/point_shadows_diagram.png)

가장 간편한 방법으로 *깊이 큐브맵* 을 얻기 위해서는 각 방향에 대해 6번 Depth Buffer 을 얻어야 한다. 하지만 일일히 for 문으로 큐브맵 한면을 얻을려고 한다면 병목도 생기거니와 코드가 잡다하게 길어질 수 있다. 이 방법 말고, **Geometry shader** 을 이용해 현재 지정된 레이어를 바꿔 한번만에 큐브맵의 값을 갱신할 수 있다.

우선, 큐브맵을 만들고, 이 큐브맵 각각의 텍스쳐를 `GL_DEPTH_COMPONENT` 로 Depth 값을 받을 수 있도록 설정한다. 또한 속성도 적절하게 설정한다. 여기서는 `GL_NEAREST` 와 `GL_CLAMP_TO_EGDE` 가 좋을 것이다. (이전 처럼 `GL_CLAMP_TO_BORDER` 도 좋을듯) 그리고 만들어진 Cubemap 값을 Framebuffer 에 붙인다.

기본적인 골자는 다음과 같다.

``` c++
// 1. first render to depth cubemap
glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ConfigureShaderAndMatrices();
    RenderScene();
glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 2. then render scene as normal with shadow mapping (using depth cubemap)
glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
ConfigureShaderAndMatrices();
glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
RenderScene();
```

### Light space transform

Point light 가 갱신할 큐브맵을 온전히 갱신하기 위해서는, 넓은 시야각을 Projection matrix로 줘야한다. 또한 빛이 사용할 쉐이더의 지오메트리 단계에서 각 Face 마다 반복적으로 깊이 값을 넣어주기 위해 빛의 각 면을 바라보는 View matrix 가 6개 필요하다.

``` c++
projection{ glm::perspective(glm::radians(90.f), 1.0f, 1.0f, far) } {

std::array<glm::vec3, 6> direction = {
    glm::vec3{1, 0, 0}, glm::vec3{-1,0, 0}, glm::vec3{0, 1, 0},
    glm::vec3{0,-1, 0}, glm::vec3{0, 0, 1}, glm::vec3{0, 0,-1}
};

std::array<glm::vec3, 6> y_pos = {
    glm::vec3{0,-1, 0}, glm::vec3{0,-1, 0}, glm::vec3{0, 0, 1},
    glm::vec3{0, 0,-1}, glm::vec3{0,-1, 0}, glm::vec3{0,-1, 0}
};

for (size_t i = 0; i < view.size(); ++i) {
    view[i] = glm::lookAt(position, position + direction[i], y_pos[i]);
    matrix_T[i] = projection * view[i];
}
```

### Depth shaders

이제 빛이 만들어내는 깊이 값을 가진 큐브맵을 만들기 위해서 쉐이더를 써야한다. 여기서 각 프래그먼트의 좌표와 빛의 좌표 및 빛의 각 큐브맵 면의 매트릭스 $$ \mathbf{T} $$ 을 쓸 예정이다. 여기서 중요한 부분은 지오메트리 쉐이더 부분이다.

``` c++
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 FragPos;
uniform mat4 lightMatrix[6];

void main() {
	for (int face = 0; face < 6; ++face)  {
		gl_Layer = face;
		for (int i = 0; i < 3; ++i) {
			FragPos = gl_in[i].gl_Position;
			gl_Position = lightMatrix[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
```

각 큐브맵에 새겨질 프리미티브 역시 삼각형일 뿐더러, 총 6번 삼각형을 그려야 하기 때문에 max_vertices 가 18이다. `gl_Layer` 라고 하는 변수는 지오메트리 쉐이더의 **built-in 변수**로,  이 변수를 통해 *레이어드 렌더링* 이 가능하다. 

`gl_Layer` 변수는 `EmitVertex()` 혹은 `EndPrimitive()` 함수를 통해 만들어진 프리미티브가 어느 레이어드 이미지로 갈 것인지 알려준다. 큐브맵에 레이어드 렌더링을 할 경우에는 이 `gl_Layer` 가 큐브맵의 각 Face 면의 위치를 가리키도록 설정된다.

그리고 Fragment Shader 에서는 built-in 변수인 `gl_FragDepth` 을 통해서 프래그먼트의 깊이를 직접 설정한다. (왜냐면 Perspective 투영 배열의 경우에는 깊이 값이 급격한 곡선을 이루기 때문?) `farPlane` 변수는 현재 빛이 투영될 공간의 끝거리이다.

``` c++
#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
	float lightDistance = length(FragPos.xyz - lightPos);
	lightDistance /= farPlane;

	gl_FragDepth = lightDistance;
}
```

이제 깊이 큐브맵의 값을 골라 그림자를 렌더링하는 법은 거의 이전 튜토리얼에서 했던 Cubemap 렌더링과 별 다른 바 없다.

![img](Images/Ch26/depthmap.png)

## Omnidirectional Shadow maps

만들어진 큐브맵을 일반 쉐이더에 적용해서, 그림자를 만드는 법은 전 튜토리얼과 별 다를 바 없다. 하지만 여기서는 큐브맵을 쉐이더에 올려야 하기 때문에 `sampleCube` 을 쓴다. 기본적인 골자는 다음과 같다.

1. 빛의 위치와 현재 프래그먼트의 위치 차이를 계산한다.
2. 차이 벡터 (방향 벡터이지만 평준화는 하지 않음) 를 텍셀 좌표로 활용해서 저장한 깊이 값을 불러옴 $$ [0, 1] $$.
3. 깊이 값을 `farPlane` 을 곱해서 실제 빛으로부터 가장 가까운 점까지의 거리를 가져옴.
4. 이 점과 차이 벡터를 비교해서 차이 벡터가 크면 그림자, 아니면 빛을 반환함.

``` c++
float ShadowCalculation(vec3 fragPos) {
	vec3 frag_to_light	= fragPos - pointlight.position;

	float current_depth = length(frag_to_light);
	
	float shadow = 0.0f;
	float bias = 
      	max(0.25 * (1.0 - dot(normalize(-frag_to_light), normalize(gs_in.Normal))), 0.05);
	
	float amnt = 1 / 48.f;
	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			for (int z = -1; z <= 1; ++z) {
				vec3 offset = vec3(x * amnt, y * amnt, z * amnt);
				float closest_depth = 
                  	texture(shadowMap, frag_to_light + offset).r * farPlane;
				
				if (current_depth - bias > closest_depth)
					shadow += 1.0f;
			}
		}
	}

	shadow /= 32.0f;
	return shadow;
}
```

위에서 PCF 도 같이 구현되어 있다.

![img](Images/Ch26/point_shadow.png)

