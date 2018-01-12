# Chapter 23. Blinn-Phong

Phong 조명 기법은 퐁 라이팅 모델의 확장이나 최적화된 모델이라고도 할 수 있다. 또한 Phong 라이팅 기법의 고질적인 문제점을 해결할 수 있다는 강점이 있다. 

![Image of Phong's reflection vectors being incorrect when larger than 90 degrees](https://learnopengl.com/img/advanced-lighting/advanced_lighting_over_90.png)

기존 Phong Lighting 은 Specular Light 에 한해서 비교적 사실적이지 못한 단점이 있고, 빛과 같은 방향으로 바라보았을 때, 제대로 조명이 반영이 안되는 단점이 있다. 하지만 1977년에 나온 **Blinn-Phong Lighting**은 Specular 조명 (정반사)을 좀 더 현실적으로 구현이 가능하다는 장점이 존재하고, 기존 Phong 과 거의 흡사한 조명을 제공할 수 있다.

다만 이 조명 기법은 Specular Lighting 에서만 제대로 적용이 가능하다.

![Illustration of Blinn-Phong's halfway vector](https://learnopengl.com/img/advanced-lighting/advanced_lighting_halfway_vector.png)

Blinn-phong lighting 은 프래그먼트로부터의 빛 방향 벡터와 시야의 방향 벡터를 이용해 $$ \overline{H} $$ (halfway vector) 을 만들어낸다. $$ \overline{H} $$ 와 $$ \overline{N} $$ 의 내적으로 산출된 $$ \cos{\theta} $$ 의 값을 인자로 받아서 정반사 조명 효과를 쉽게 만들 수 있다.

Halfway vector 을 구하는 법은 다음과 같다.
$$
\overline{H} = \frac{\overline{L} + \overline{V}}{\|\overline{L}+\overline{H}\|}
$$
기술적으로는 퐁 공식을 사용하는 계산이라면 어디서든 이 계산을 적용할 수 있다. (Specular 등...) 만약 삼각형이 표현하는 프래그먼트의 표면의 곡률이 상대적으로 작거나, 삼각형이 상대적으로 작으면 $$ \overline{H} $$ 을 그냥 Vertex & Geometry 쉐이더에서 계산해도 무방하다.

그리고 Blinn-phong 의 단점은 Phong 의 조명을 구현하기 위해서는 `intensity` 계수를 높여야 한다는 점이다.

![img](images/Ch23/blinn_phong.png)

위가 Blinn-phong 이고, 밑이 일반 Phong 이다. 계수는 같다.

![img](images/Ch23/phong.png)

