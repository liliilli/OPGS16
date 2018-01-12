# Chapter 22. Anti Aliasing

**Aliasing** 이라는 개념은, 어떤 물체를 스크린 상에 출력했을 때 경계점이 끊어보이고, 픽셀 조각이 튀어보이는 현상을 일컫어 Aliasing 이라고 말한다. 따라서 **Anti-Aliasing** 은 경계점을 흐릿하게 하면서 픽셀 조작을 튀게 하는 것을 막는 기법이다.

먼저 **SSAA(Super-Sampling Anti Aliasing)** 이라고 하는 기존 해상도보다 세부적으로 큰 해상도를 쓴 뒤에 기본 프레임 버퍼에 출력을 할 때 낮은 해상도로 다운샘플링을 해서 AA 을 해결하는 기법이 존재한다. 하지만 이 방법은 더 큰 해상도를 쓰기 때문에 성능 저하가 일어날 수 있는 문제점이 존재한다.

그래서 이 기법을 해결하기 위해 쓰일 수 있는 것이 **MSAA(Multi-sample AA)** 라는 기법이다. 이 기법은 픽셀을 샘플링하는데 좀 더 효율적인 방법을 쓴다. 또한 MSAA 는 기본적으로 OpenGL 에 built-in 되어있다.

## Multisampling

Multisampling 이 무엇이고 어떻게 구현되는지 알아보기 위해서 OpenGL 의 Rasterizer 단계를 세부적으로 보기로 한다.

Rasterizer 는 버텍스와 지오메트리 쉐이더를 거친 최종 형태의 정점과 Fragment Shader 사이에서 진행되는 알고리즘의 조합들로 이루어진 단계다. Rasterizer 는 한 기초 프리미티브에 필요한 정점 집합을 가져와서 이를 Fragment 의 집합으로 변환한다. (한 프리미티브에서 여러 프래그먼트가 나올 수 있다.)

정점 좌표는 실수를 포함한 수 많은 값을 가질 수 있지만 Fragment 는 최종 표시될 윈도우의 좌표 중 하나가 아니면 안되기 때문에 정점과 정점 사이의 수 많은 정점들이 하나의 픽셀이 될 수는 없다. (one-on-one 이 아니다) 그래서 Rasterizer 는 정점을 이루는 프리미티브가 고정될 프래그먼트의 픽셀이 어느 픽셀이 될 지를 **샘플링** 해야한다.

![Image of a triangle being rasterized in OpenGL](https://learnopengl.com/img/advanced/anti_aliasing_rasterization.png)

위의 이미지는 Rasterizer 의 샘플링하는 과정을 시각화 한 것이다. 삼각형 프리미티브의 중앙의 빨간색 점들은 **Sample Point** 라고 한다. 이 Sample point 을 따라서 프래그먼트가 만들어지게 된다. 따라서 일단 이 과정을 거쳐서 나온 (렌더링된) 프래그먼트는 다음과 같을 것이다.

![Filled triangle as a result of rasterization in OpenGL](https://learnopengl.com/img/advanced/anti_aliasing_rasterization_filled.png)

우리가 기대했던 것과는 달리 삼각형이 매우 비뚤배뚤한 것을 알 수 있다. 어느 부분은 꼭짓점까지 처리된 반면에 어느 부분은 뭉툭하게 변해버렸다. 이렇게 픽셀이 튀어보인다던가, 의도치 않게 모양이 변형되버린 현상을 **Aliasing** 이라고 부른다.

*멀티 샘플링* 은 한번만 샘플링을 시도하지 않고, 이름답게 여러번 정점 픽셀의 주위를 샘플링해서 최종 픽셀을 결정해낸다. 대개 MSAA 는 4개의 **Subsample**을 사용해서 최종 픽셀을 결정해낸다. 이 말인 즉슨, 외곽선의 한 픽셀을 샘플링 하는데 4번 처리를 한다.

![Multisampling in OpenGL](https://learnopengl.com/img/advanced/anti_aliasing_sample_points.png)

> 멀티 샘플 포인트가 많아질 수록, 더 정확한 AA 효과를 낼 수 있다.

그러면 멀티 샘플링에 대해서 Fragment Shader 는 어떤 식으로 처리해서 최종 색을 반환할 수 있을까? 여기서 염두해야 할 점은, MSAA 에서 Fragment Shader 는 한 픽셀 당 한번씩만 처리를 한다는 점이다. 따라서 Multisample 의 횟수는 아무런 상관이 없다.

Fragment Shader 는 대신에 보간된 각 정점 픽셀의 **중간점**을 데이터 삼아서 처리하며 결과로 나온 색은 각 서브샘플에 저장된다. 각 픽셀의 서브샘플들에 우리가 렌더링한 프리미티브의 색들이 저장되면 이 컬러 값들의 평균 값을 가진 색이 최종 색으로 결정된다.

따라서 이 방법을 통해 프리미티브의 가장자리는 Anti-aliasing 을 구현하게 된다. 

![Rasterization of triangle with multisampling in OpenGL](https://learnopengl.com/img/advanced/anti_aliasing_rasterization_samples.png)

위의 이미지로 예시를 들어보자. 파란색 샘플은 삼각형 안쪽에 있기 때문에 색이 입혀졌고, 가장자리쪽의 그렇지 않은 부분은 색이 다른 색(Clear color)로 입혀진 것을 알 수 있다. 삼각형 안쪽의 모든 픽셀 지역은 셰이더를 한번만 수행해서 최종 색을 뽑아낸다. 삼각형 가장자리의 샘플 중 일부분만 값이 입혀진 픽셀 지역은 색이 입혀진 서브 샘플의 양에 기반해서 최종 색 값을 평균치로 보간해서 반환한다.

![Rasterized triangle with multisampling in OpenGL](https://learnopengl.com/img/advanced/anti_aliasing_rasterization_samples_filled.png)

Multi-Sampling 을 통해서 컬러 값을 보간할 수 있을 뿐만 아니라, Depth 와 Stencil Testing 역시 멀티 샘플링을 이용할 수 있다. **Depth Testing** 에서는 $$ 0.0 $$ 에서 $$ 1.0 $$ 사이의 정점의 깊이 값이 멀티 샘플링에 의해서 보간될 수 있다. **Stencil testing** 에서도 스텐실 버퍼의 값이 멀티 샘플링에 의해 보간될 수 있다. 이는 곧 Depth 혹은 Stencil buffer 의 크기 역시 샘플링 수에 따라서 늘어난다.

사실 여기에서 말하는 Rasterizer 는 보다 복잡하지만 이 부분에 관한 사항은 개개인에게 알아서 조사하는 것으로 맡긴다.

## MSAA in OpenGL

