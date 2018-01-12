# Chapter 25. Shadow Mapping

그림자는 폐색 (occlusion) 에 의해서 빛이 못 통과함으로써 만들어진다. 만약 어떤 객체들이 여러 위치가 있고, 어떤 한 점의 빛 (Point light) 이 객체에 막혀서 그 뒤의 객체를 못 비춘다고 하면  이 객체는 그림자에 존재하게 된다. 3D 게임에서는 이 그림자라는 존재 (2D도 그렇거니와) 가 깊이감 형성에 매우 큰 영향을 미치기 때문에 필수적이라고 볼 수 있다.

![comparrison of shadows in a scene with and without in OpenGL](https://learnopengl.com/img/advanced-lighting/shadow_mapping_with_without.png)

아직 완전하게 그림자를 구현할 수 있는 기법은 Legacy Rendering 상에서는 존재하지 않기 때문에, 여러 가지 그림자 구현법이 알려져 있다. 이 튜토리얼에서 쓸 그림자 구현 기법은 **Shadow Mapping** 이다. 맵핑을 이용해 그림자를 구현하는 기법은 그림자 구현법 중에서 상대적으로 쉬운 편에 속한다.

## Shadow Mapping

![Shadow mapping illustrated.](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory.png)

우리가 만약 빛의 입장이 되어서 모든 물체를 본다고 생각하면, 빛의 입장에서 보이는 물체들은 다 **밝고**, 안보이는 물체들은 다 **그림자에 가려 어두울** 것이다. 이는 위 그림과 같다. 빛의 영향을 받아 밝게 보일 수 있는 물체 표면은 파란색이고, 폐색되어 그림자에 가리워진 표면들은 검은색이다. 

만약 빛에서 나오는 광선 (**ray**) 들을 기준으로 해서 모든 공간의 점들을 계산할려고 하면 실시간 렌더링에서는 매우 성능이 좋지 않을 것이다. 그러나 *빛의 시점에서 공간을 본다는 개념*을 빌려서 대신 이용할 수 있는 버퍼가 존재한다. Shadow mapping 을 할 때 **빛의 시점에서 본 Depth buffer** 을 이용한다.

*Depth Buffer* 은 카메라의 시점에 따라서 각 프래그먼트의 값들이 $$ [0,1] $$ 사이의 값들로 정해진다. 빛에서 본 공간을 off-screen rendering 을 통해 Render buffer 에 저장한 Depth buffer 로 저장한다. 그 뒤에 Render buffer 에 저장된 값을 보면 빛에서 가장 가까운 (**빛의 영향권에 드는**) 프래그먼트의 Depth 값만이 저장된 것을 알 수 있을 것이다. Texture 혹은 Render buffer 에 저장된 Depth 값들을 **Depth map 혹은 Shadow map** 이라고 한다.

![Different coordinate transforms / spaces for shadow mapping.](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory_spaces.png)

왼쪽 이미지는 Directional Light 을 쬐었을 때 그림자가 나타나는 형태를 보여준다. 직사광선에서  찍은 Depth map 을 이용해서 빛의 영향권을 받는 (가장 가까운) 지점을 알 수 있고, 결과로 어느 프래그먼트가 그림자 안에 있는지를 알 수 있다. 이를 구현하기 위해서는 model, view, 빛 시점에서 바라보는 projection matrix 가 필요하다. World 공간의 모든 좌표들을 빛이 바라보는 최종 시점의 좌표로 변환시키는 매트릭스를 $$ \mathbf{T} $$ 라고 한다. ( $$ \mathbf{T} = \mathbf{P}\mathbf{V} $$ )

오른쪽 그림은 카메라(유저) 시점에서 바라본 영상을 보여준다. 물체 뒤의 좌표 $$ \overline{P} $$ 가 **그림자**에 존재한다는 것을 나타내야 한다. 우선 첫번째로 World 공간 좌표 $$ \overline{P} $$ 을 $$ \mathbf{T} $$ 을 이용해 변형해서 $$ \overline{P} $$ 의 $$ z $$ 값을 $$ 0.0 $$ 에서 $$ 1.0 $$ 사이로 나타내게 할 수 있다. 그러면 두번째로 $$ z $$ 값을 빛의 시점에서 렌더링해서 찍어낸 **Depth map** 의 적절한 값을 비교한다. 만약 비교했을 때 $$ z $$ 값이 크면 그림자에 있다고 판단할 수 있다.

1. 빛의 시점에서 off-screen rendering 을 해서 Depth map 을 저장한다.
2. 사용자 시점에서 바라본 특정 임의 좌표를 $$ \mathbf{T} $$ 을 이용해 변형한 뒤, Depth value 와 비교한다.

## The depth map

우선 사용자 정의 프레임 버퍼에서 off-screen rendering 을 이용해 Depth value 을 텍스쳐 버퍼에 갱신해야 한다. 코드는 다음과 같다.

``` c++
// Generate Frame buffer
unsigned int depthMapFBO;
glGenFramebuffers(1, &depthMapFBO); 

// Create Depth buffer
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMap;
glGenTextures(1, &depthMap);
glBindTexture(GL_TEXTURE_2D, depthMap);
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
// GL_REPEAT 는 GL_CLAMP_TO_BORDER 로 바꿔쓰는 것이 낫다.

// Bind texture buffer to frame buffer.
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
glDrawBuffer(GL_NONE);
glReadBuffer(GL_NONE);
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

텍스쳐의 크기는 실제 화면 사이즈가 될 필요가 없다. 오히려 크면 클 수록 그림자를 세밀하게 표현할 수 있다. 그리고 밑쪽 구문에서 `glDrawBuffer` 와 `glReadBuffer`가 `GL_NONE` 으로 설정되어 있는데, 왜냐면 현재 프레임버퍼에서는 어떠한 컬러 값도 렌더링하지 않을 것이기 때문이다. (Depth 값만 렌더링한다)

설정이 다 됬으면 이전 튜토리얼처럼 프레임 버퍼를 바인딩한 후에 렌더링을 해서 Depth value 을 갱신한다. 여기서 `glViewport` 함수로 최종 스크린 공간 사이즈를 프레임버퍼의 크기에 맞게 갱신해서 올바른 값들이 들어갈 수 있도록 한다. 또한 현재 프레임 버퍼는 Depth 값만을 갱신할 것이기 때문에 `glClear` 로 Depth Buffer 을 우선 초기화한 뒤에 렌더링한다.

### Light space transform

이제 직접 빛을 쬐서 어떻게 Depth value 가 어떻게 갱신되는 지를 한번 볼 것이다. 이 때 사용할 빛은 **직사광선** 이다. 정점 좌표의 변형에 쓰일 $$ \mathbf{T} $$ 을 만들어야 하기 때문에, 직사각형은 쬐는 빛의 방향이 평행이므로 **glm::ortho** 을 써서 Projection matrix 을 만든다. 그리고 View matrix 는 **glm::lookAt** 을 써서 뷰 매트릭스를 만든다.

``` c++
auto light_position = glm::vec3(-4.0f, 4.0f, -4.0f);
DirectionalLight light_directional{
    glm::normalize(glm::vec3(0) - light_position),
    glm::vec3(1) * 0.2f, glm::vec3(1), glm::vec3(1),
};
glm::mat4 light_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 12.5f);
glm::mat4 light_view    = glm::lookAt(light_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
glm::mat4 light_matrix  = light_projection * light_view;
```

### Render to depth map

Vertex Shader 에는 빛의 시점에서 본 좌표를 담는다. 컬러 정보를 텍스쳐에 담을 일은 없기 때문에 Fragment Shader 는 메인 함수를 빈 채로 둔다. 스크린 밖 렌더링을 한 뒤에 Depth value가 담긴 텍스쳐를 기본 프레임 버퍼에서 출력하면 다음과 같이 나올 것이다.

![img](images/Ch25/depthvalue.png)

직사각형의 Depth value 는 Linear 하기 때문에 값에 비례해서 거리 역시 비례하다. 가장 검은 쪽이 가장 가깝고, 가장 흰 쪽이 가장 먼 것을 알 수 있다. 

### Rendering Shadows

우선 World 공간의 좌표 $$ \overline{P} $$ 가 그림자에 위치해 있는가를 확인할려면, $$ \mathbf{T} $$ 을 곱해 빛의 시점에서 Depth value 가 사상된 위치의 Depth value 보다 큰 지 (그림자에 위치해 있는지), 아니면 작은 지 (빛의 영향을 받고 있는지) 를 확인해야 한다. 그럴려면 Vertex Shader 에서 Model matrix 에 사상된 각 정점 $$ \overline{P} $$ 을 $$ \mathbf{T} $$ 와 곱한 뒤에 Fragment Shader 로 보낸다. 그리고 이 보낸 좌표 값을 Depth Map 의 텍셀의 값과 비교해서 그림자인가 아닌가를 반환한다. 이 반환한 값을 *Diffuse 및 Specular* 에 곱해서 적용하면 기본적인 그림자가 만들어진다.

다만 여기서 주의해야 할 점은, Orthogonal Perspective 에서는 $$ \mathbf{T} $$ 에 의해서 변형된 모든 값 (Clipping 제외)이 $$ [-1, 1] $$ 사이에 있기 때문에 파이프라인 단계에서 Clip space 로 변형 시 $$ w $$ 축 값으로 $$ x,y,z $$ 을 나눠주는 것을 명시적으로 할 필요는 없다는 점이다. 그러나 일반 점 조명 (Point light) 혹은 Specular light 와 같은 경우에는 이 과정이 별도로 필요하다.

그리고 $$ fragPosLightSpace $$ 의 값은 $$ [-1, 1] $$ 값 사이인데, Depth map 의 좌표는 $$ [0, 1] $$ 사이이고 Depth value 역시 $$ [0, 1] $$ 의 범위를 가지므로 간단한 맵핑이 다시 필요하다.

``` c++
float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // ...
  	projCoords = projCoords * 0.5 + 0.5; 
}
```

맵핑된 값의 $ z $ 값이 현재 좌표의 Depth value 을 가지고 있기 때문에 이것을 깊이 맵의 값과 비교한다.

``` c++
float closestDepth = texture(shadowMap, projCoords.xy).r; 
// get depth of current fragment from light's perspective
float currentDepth = projCoords.z;
// check whether current frag pos is in shadow
float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

return shadow;
```

다음과 같이 약간 뭔가 이상하지만 어쨌든 그림자가 나오는 것을 알 수 있다.

![img](images/Ch25/acne.PNG)

### Shadow Acne

위 결과 화면을 보면 그림자가 분명 보이기는 하나 옛날 주택의 창문에서 보이던 특유의 무늬가 잔뜩 나타난 것을 알 수 있다. 자세히 보면 이렇게 직선으로 그림자가 진 것을 알 수 있다.

![Image of shadow acne as Moiré pattern with shadow mapping](https://learnopengl.com/img/advanced-lighting/shadow_mapping_acne.png)

이렇게 그림자가 지는 이유는 Depth value 을 버퍼에 샘플링하면서 떨어지는 정확도로 인해 발생되기 때문이다. 만약 1024x1024 픽셀의 깊이 맵이 있다고 할 때 무수한 프래그먼트와 정점에 의해서 발생되는 실제 깊이 값의 크기는 어마무지할 것이다. 그러나 이 값들을 다 버퍼에 때려박아 내놓을 수는 없는 노릇이기 때문에 (메모리, 성능 이슈) 일정 사이즈로 제한한 깊이 맵에 Depth value 을 샘플링해서 집어넣는다.

그러나 이 깊이 맵을 이용해서 그림자를 구현할 때는 무수한 프래그먼트 좌표 혹은 정점 좌표가 변환되서 나온 깊이 값 $$ z $$ 을 해상도가 제한된 깊이 맵의 특정 텍셀에 비교하기 때문에 분명 햇빛의 영향권인데도 불구하고 그림자가 지게 된다.

![Shadow acne explained](https://learnopengl.com/img/advanced-lighting/shadow_mapping_acne_diagram.png)

이 방법을 해결하는 아주 간단한 방법이 있다. 임의의 좌표가 매트릭스에 변환되서 나오는 $$ z $$ 깊이 값을 아주 약간만 올려주면 된다. 약간만 올려주는 이 값을 **Shadow Bias** 이라고 한다.

![Shadow mapping, with shadow acne fixed using shadow bias.](https://learnopengl.com/img/advanced-lighting/shadow_mapping_acne_bias.png)

다만 이 Bias 값이 많이 높아지면 깊이 값 $$ z $$ 가 Depth map 에 저장된 값보다 역전되서 낮아져, 그림자 처리가 안되서 약간 이상하게 그림자가 지는 (특히 바닥면과 닿은 모서리쪽) **Peter Panning** 현상이 일어날 수 있다. 때문에 가급적이면 다음과 같이 공식을 짜서 각 프래그먼트마다 다른 bias 값을 쓰는 것이 좋다.

``` c++
float bias	= max(0.0175 * (1.0 - dot(lightDir, normal)), 0.0025)
```

면의 곡률이 높아지면 높아질 수록 Acne가 일어나기 쉽다. 빛의 방향과 프래그먼트가 평행이 될 수록 Acne 가 일어나기 쉬워진다. 그래서 위와 같이 내적을 이용해 bias 을 높이거나 줄여서 *Shadow Acne* 현상을 방지한다.

**Peter Panning** 현상을 방지하는 데는, Face Culling 을 쓰는 것 또한 좋을 수도 있다. 그러나 이 방법은 안이 뚫린 오브젝트에는 전혀 안 먹히는 단점이 존재한다.

![img](images/Ch25/peterpanning.png)

### Over Sampling

![img](images/Ch25/repeat_problem.png)

위 화면에의 왼쪽 끝을 보면, 그림자가 생길 물체가 없는데도 그림자가 생긴 것을 알 수 있다. 사실 이 그림자는 오른쪽 물체에 생긴 그림자와 똑같다. 두번째 문제로 물체 뒤로 엄청 거대한 그림자가 생긴 것을 알 수 있다.

첫번째 문제는 Depth map 의 텍스쳐 속성이, 좌표를 넘어갈 때 텍스쳐가 반복되서 나타나기 때문이다. 따라서 다음과 같이, `GL_TEXTURE_WRAP_S,T` 속성을 `GL_CLAMP_TO_BORDER` 로 변환한다.

``` c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
```

그리고 두번째 문제는 그림자를 진 부분의 `z` 값이 (Depth value 가) $$ 1.0 $$ 을 넘어가기 때문이다. 사실 Projection matrix 의 far 을 좀 더 크게 설정하면 그림자가 차지하는 부분이 줄어든다. 하지만 언제까지나 far 을 100.0f 와 같이 잡을 수는 없으니 Fragment Shader 에서 `z` 값이 1.0 을 넘으면 그림자가 없도록 설정한다.

다만 이 두 가지 해결책은 경계면 밖으로 어떠한 장애물도 없거나 그림자의 영향을 받지 않는 물체가 있어야 한다는 점이 제한조건으로 작용한다.

### PCF

![img](IMages/CH25/PCF.png)

화면을 좀 더 확대해서, 세부 그림자를 보면 간혹가다 저렇게 지터링이 일어나는 그림자를 볼 수 있다. 이유는 *Shadow Acne* 와 비슷하게 Depth map 의 해상도와 Fragment 의 깊이 값 대조 정확도 문제로 발생한다. Depth map 의 해상도를 올리면 이 현상은 해결될 수 있지만 성능 이슈가 따른다.

그럴바에야 차라리 경계부분을 뭉개버리는 게 좋을 것 같다. 이전에 배운 MSAA 와 같이 멀티샘플링을 이용해 블러링을 한다던가, 아니면 푸아송 알고리즘을 적용해 쓰는 법도 있다. 여기서는 **PCF(Percentage-closer filtering)** 방법을 써서 그림자를 블러링해보기로 한다.

이 방법은 기준이 되는 텍셀 위치에서 좌우상하, 또는 9방향 주위의 텍셀을 렌더링해서 평균값을 내어 쉐도우 값 ( $$[0, 1]$$ ) 을 반환해 적용하는 방식이다.

``` c++
if (current_depth <= 1.0f) {
	vec2 texel		= 1.0f / textureSize(shadowMap, 0);
	float bias		= max(0.0175 * (1.0 - dot(lightDir, normal)), 0.0025);

	for (int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcf_depth = texture(shadowMap, proj_coord.xy + vec2(x,y) * texel).r;
			shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0f;
}
```

`texel` 은 기준 텍셀에서 어느 정도 거리의 주위 텍셀을 샘플링해서 보간할지 정하는 값을 가진다. 이 값이 클 수록 블러링이 될 확률이 높아지거나 정확도가 떨어질 수 있다. 하지만 기준 텍스쳐 크기보다 값이 매우 작으면 적용이 안될 가능성이 있다.

따라서 최종 결과물은 다음과 같다.

![img](images/Ch25/ortho.PNG)

## Orthographic vs Perspective

직사광선이 왜 Orthographic Projection 을 쓰냐면, Orthographic 은 캐치한 값들을 `w` 값에 따라서 맘대로 변형하지 않기 때문이다. 그러나 Perspective 는 파이프라인에서 Clip space 로 좌표를 변형하기 위해 `w` 값에 따라 좌표 값들이 변할 뿐더러 시점에 따라서도 좌표 결과가 크게 달라질 수 있다. 따라서 후자는 Point light나 Spotlight 에 적합하다.

또한 Orthographic 과 Perspective 에서 산출되는 Depth value 의 선형성 역시 다르다. Orthographic 은 선형적인데 반해 Perspective 는 다음 식을 따르므로,

$$
\mathbf{F}_{depth} = \frac{\frac{1}{z} - \frac{1}{near}}{\frac{1}{far} - \frac{1}{near}}
$$
Depth 맵을 그대로 화면에 표시하면, 거의 흰색밖에 보이지 않을 것이다. 만약 Perspective 을 써서 그림자를 구현한다고 하면 이 값들을 선형화 시켜줘서 $$ \overline{P} $$ 의 변형된 좌표의 $$ z $$ 값과 비교를 해야할 것이다.

``` c++
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
```

$$
\mathbf{F}_{lineardepth} = \frac{2\mathbf{N}\mathbf{F}}{\mathbf{F}+\mathbf{N}-(2\mathbf{F}_{non}-1)(\mathbf{F}-\mathbf{N})}
$$

