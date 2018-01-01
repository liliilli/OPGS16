# Chapter 14. Blending

* **Blending**
  OpenGL 에서의 Blending 은 object 에 **투명도**를 구현하는 방법으로 알려져있다. 투명도라는 것은 어떤 물체가 빛을 완전히 반사시키지 않고 투과시켜서 뒤에 보이는 물체의 색과 같이 섞이는 정도를 말한다.

예를 들면, 색이 입혀진 유리의 경우는 투명한 물체이다. 만일 유리가 자기 자신만의 색을 가지고 있다 한들, 우리에게 보이는 색은 뒤의 물체의 고유색과 섞인 복잡한 색이 될 것이다.

![Image of full transparent window and partially transparent window](https://learnopengl.com/img/advanced/blending_transparency.png)

이 투명도는 OpenGL 혹은 그래픽스 영역에서는 `alpha` 값이라고 부른다. RGBA 에서 4번째 값을 차지하는 alpha 값은 $ 0.0 $ 에 가까워 질수록 완전히 투명하게 되며, $ 1.0 $ 에 가까워 질수록 불투명 (opaque) 하게 된다. 우선 블렌딩이 아닌 약간 무식한 방법을 시험해본다.

## Discarding Fragments

블렌딩을 이용해서 써먹을 수 있는 기법 중 하나는, 2D 텍스쳐로 만들어진 풀이 있다고 하면, 이 풀 주위의 풀이 아닌 영역들을 날려버리는 것이 있다. 풀 같은 경우에는 일반적으로 큐브로 만들지 않고, 2D 평면을 세운 뒤에 그것에 풀 텍스쳐를 발라버리는 형태로 만들어지는 것이 보통이다.

![Texture image of grass with transparency](https://learnopengl.com/img/textures/grass.png)

위 이미지는 풀을 제외한 모든 영역의 alpha 값이 $ 0.0 $ 임을 알 수 있다. GLSL 의 함수 `discard` 을 통해서 Fragment shader 에서 해당 영역의 프래그먼트들을 버릴 수 있다.

우선 Texture 을 불러와서 다음과 같이 필요하지 않은 부분을 Discard 하는 쉐이더 프로그램을 짠다.

``` c++
#version 330 core
out vec4 FragColor;
in	vec2 TexCoord;
in	vec3 FragmentPos;
in	vec3 Normal;

// Uniform variables
uniform sampler2D texture_diffuse1;

void main() {    
	vec4 texColor = texture(texture_diffuse1, TexCoord);
	if (texColor.a < 0.1f)
		discard;

	FragColor = texColor;
}
```

이 경우에, 만약 프래그먼트의 alpha 값이 0.1 보다 작으면 해당 프래그먼트는 쉐이더 프로그램을 멈추고 아예 안 보이게 폐기처분 된다.

다음은 결과 화면이다.

![img](14_discard.png)

>만약 풀과 같은 완전 투명하고 완전 투명하지 않은 무언가를 텍스쳐로 받아서 샘플링 할 때, 파라미터를 정하지 않으면 OpenGL 은 보더 영역의 값을 **다음에 반복되는 텍스쳐의 값** 을 토대로 보간해서 최종 결과를 내놓게 된다. 따라서 약간의 붕 떠있는 색상 값같은 것이 존재할 수 있다.
>
>이를 막기 위해서는 `GL_CLAMP_TO_EDGE` 등의 옵션을 써서 투명한 값이 그대로 지속되록 해야한다.
>
>``` c++
>helper::Texture2D texture_grass{ "14_textures/grass.png", GL_RGBA };
>texture_grass.SetTextureParameterI({
>    std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE),
>    std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) });
>```

## Blending

풀 처리 처럼 프래그먼트를 아예 버려서 투명한 부분을 날려 버리는 것도 좋은 방법이지만, 점진적으로 알파값이 변한다고 할 때 이에 대해서 일종의 효과를 내기 위해서는 `discard` 만으로는 구현하는 것이 매우 힘들다.

따라서 OpenGL 은 `glEnable(GL_BLEND)` 로 반투명한 텍스쳐에 대해서 블렌딩을 할 수 있도록 지원해준다. OpenGL 의 기초 블렌딩 방정식은 다음과 같다.
$$
\overline{C}_{result} = \overline{C}_{source} * {F}_{source} + \overline{C}_{destination} * {F}_{destination}
$$

* $$ \overline{C}_{source} $$ : 소스 프래그먼트의 알파값을 포함한 컬러 벡터.
* $$ F_{source} $$ : 소스 프래그먼트의 요인 값. 설정에 의해서 바뀔 수 있다.
* $$ \overline{C}_{destination} $$ : 타겟 프래그먼트의 알파값을 포함한 컬러 벡터.
* $$ F_{source} $$ : 타겟 프래그먼트의 요인 값. 설정에 의해서 바뀔 수 있다.

모든 프래그먼트의 프래그먼트 쉐이더가 다 처리된 후에야 **Blending** 과정이 일어나게 된다. 이 과정에서 투명값을 가지고 있고 카메라에서 뻗어나가는 축에 비독립적인 컬러 벡터들이 대상이 된다.

기초 Blend 방정식은 최종 Fragment 의 색 결과를 잠시 보류해서 블렌딩을 거친 후에 나온 결과값을 Color Buffer 에 저장한다. 이 과정에서 source 와 destination 은 OpenGL 에 의해 자동으로 설정된다. 그러나 **Factor** 값은 우리가 어떻게 설정해주느냐에 따라 달라질 수 있다.

![Two squares where one has alpha value lower than 1](https://learnopengl.com/img/advanced/blending_equation.png)

만약 Source 와 Destination 컬러 벡터가 위의 두 벡터라고 가정했을 때, Red 프래그먼트 위에 Green 프래그먼트를 얹어서 블렌딩을 한다고 가정한다. 그러면 빨강 사각형은 **Destination Color** (기반이 되므로) 가 되며, Color Buffer 에 맨 처음에 존재해야 한다. 그러면 초록색 사각형은 **Source Color** 가 된다.

이 때 생각해야 할 점은, Factor value 을 어떻게 설정해줘야 하는가이다. 일단 보통은 초록색 사각형을 그대로 빨강색 사각형에 곱해서 최종 결과물을 얻고 싶다. 그러면 $$ F_{src} $$ 는 source 의 alpha 값을 그대로 따를 것이고, $$ F_{destination} $$ 은 destination 의 알파값에 source 의 알파값을 뺀 꼴로 요인 값이 산출될 것이다. 
(빨강색이 반투명이고 그 뒤에 뭔가 더 있고 하는 연속적인 상황에서도 비슷하게 돌아간다.)

따라서 방정식은 다음과 같을 것이다.
$$
\overline{C}_{result} = \begin{pmatrix} 0.0 \\ 1.0 \\ 0.0 \\ 0.6 \end{pmatrix} * 0.6 + \begin{pmatrix} 1.0 \\ 0.0 \\ 0.0 \\ 1.0 \end{pmatrix} * (1 - 0.6)
$$
$$ \overline{C}_{result} $$ 의 컬러 벡터 값은 다음과 같이 나올 것이다.

![Two containers where one has alpha value lower than 1](https://learnopengl.com/img/advanced/blending_equation_mixed.png)

그러면 이 계산을 OpenGL 에서 처리하려고 할 때, Factor 은 어떻게 설정해야 하는가? 답은 `glBlendFunc(GLenum source_factor, GLenum dest_factor)` 을 쓰면 된다. 이 함수는 소스와 목적지 벡터에 대한 요인 값을 따로따로 설정해줄 수 있다.

![img](14_factor.png)

여기서 나오는 $$ \overline{C}_{constant} $$ 는 `glBlendColor` 로 설정할 수 있는 Clamp 벡터 상수 값이다. 이 함수에서는 RGBA 값에 대해 $$ 0.0 $$ 에서 $$ 1.0 $$ 까지의 값을 설정할 수 있다.

또한 `glBlendEquation(GLenum mode)` 을 통해 값을 더할 지, 뺄 지, 아니면 목적 벡터와 소스 벡터를 치환해 뺄지를 설정할 수 있다.

* GL_FUNC_ADD
* GL_FUNC_SUBSTRACT
* GL_FUNC_REVERSE_SUBSTRACT

기본으로는 GL_FUNC_ADD 가 설정이 되어 있기 때문에 $$ \overline{F}_{result} $$ 는 두 벡터를 더한 값이 될 것이다.

## Rendering semi-transparent textures

이제 Blending 을 직접 해보기로 하자.

``` c++
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
```

Blending 은 Fragment Shader 이후에 처리되기 때문에, Fragment shader 에서 별도로 뭔가 할 필요는 없다.

![img](14_blend_failure.png)

프로그램을 실행하면 다음과 같이 블렌딩이 된 것을 확인할 수 있다. 하지만 여기서 문제점이 보이는 것 또한 알 수 있다.

이 문제가 발생하는 이유는, Depth testing 에서는 현재 대조하는 프래그먼트가 투명한 가를 고려하지 않고 투명한 부분의 보여야될 프래그먼트가 앞에 있는 다른 프래그먼트의 Depth value 로 덮어 씌워지게 된다. 따라서 투명한 부분으로 비치는 뒷 프래그먼트가 원래는 보여야됨에도 불구하고 사라지게 된다.

그래서 이 문제를 해결하기 위해서 많은 방법들이 존재한다. 첫 번째 방법으로는 가장 간단하지만 조금 더러운(?) 방법으로, Camera 의 위치와 투명한 블렌딩될 오브젝트의 위치 값 사이의 거리를 구해서, 먼 순으로 정렬한 뒤에 먼 순에 위치한 오브젝트부터 묘화하는 방법이 있다.

이 방법을 적절히 구현한다.

``` c++
// Nasty solution for resolving depth testing discarding problem.
auto greater = [&camera](glm::vec3& lhs, glm::vec3& rhs) {
    auto lhs_dist = glm::length(camera.GetPosition() - lhs);
    auto rhs_dist = glm::length(camera.GetPosition() - rhs);

    return lhs_dist > rhs_dist;
};
std::sort(grass_pos.begin(), grass_pos.end(), greater);
```

그러면 다음과 같이 문제가 해결됬음을 알 수 있다.

![img](14_blend_success.png)

하지만 이 방법으로는 많은 오브젝트를 효율적으로 정리해서 그리기에는 시간이 많이 걸린다. $$ O(n\lg{n}) $$ 이 걸릴 것이다. 따라서 이 방법 외에도 **Order independent transparency** 를 이용해 보다 효과적으로 블렌딩을 해내는 방법이 존재한다. 이 방법은 또 다른 버퍼인 **A-buffer** 사용해 구현한다.

