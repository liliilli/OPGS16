# Chapter 15. Face Culling

머릿 속에서 육면체를 그려서 한 시점에서 보이는 육면체의 면의 갯수가 몇 개까지 보일 수 있는지를 한번 생각해보면, 최대 3개가 보일 수 있음을 알 수 있다. 만약에 육면체를 다 그리지 말고, 보이는 것만 화면 상에 그릴려고 한다면 최대 50% 이상의 효율을 뽑아낼 수 있다.

하지만 OpenGL 에서는 뒷면 프래그먼트와 앞면 프래그먼트를 어떻게 골라내서 효율적으로 그릴려고 하는 것일까? 일단 뇌 속에서 가상의 형태를 구현해보기로 한다. 이 형태는 **두 면**을 가진다. 하나는 **Face** 로 카메라 혹은 유저에게 보이는 면이고, 또 다른 하나는 **Back** 으로 유저에게는 보이지 않는 면이다. 

**Face Culling** 은 이 두 면에서 유저에게 보이는 Face 면만을 골라내 이 면의 프래그먼트들만 렌더링되도록 지시한다. OpenGL 은 유저에게 보이는 Face 면만을 골라내고, 그 외의 면들은 죄다 폐기를 시킨다. 이렇게 해서 거의 2배 이상의 Fragment shader 호출 비용을 절약할 수 있게 된다. 

기본으로는 OpenGL 은 Face Culling 이 비활성화된 상태이다. 만약 Face Culling 을 쓰기 위해서는 `glEnable` 로 활성화 시켜줘야 하고, 또한 OpenGL 에게 어떤 방식으로 Face 와 Back 을 구별할 것인지도 알려줘야 한다. 구별하는 방식은, **정점 데이터들의 winding order** 을 통해 분석되어 분류될 수 있다.

## Winding order

일반 프리미티브를 정의할 때는 삼각형으로 정의하는 것이 일반적이다. 정점 데이터를 가지고 삼각형 프리미티브를 정의할 때, 두 가지 방법으로 삼각형을 만들 수 있다. **시계 방향** 과 **반시계 방향** 이다.

![Winding order of OpenGL triangles](https://learnopengl.com/img/advanced/faceculling_windingorder.png)

버텍스 데이터를 정의할 때, 3개의 정점은 하나의 프리미티브를 정의할 수 있고, 또한 반시계 방향인지 시계 방향으로 정점을 정의하는지 알 수 있다. OpenGL 은 프리미티브를 렌더링할 때 이 프리미티브가 만들어지는 **방향에 의해** 폐기를 할 수 있다. 기본으로는 OpenGL 은 *Face* 을 *Counter-clockwise* 방향으로 정의하고 있다.

따라서 **Face Culling** 을 쓰고 싶다면 삼각형을 만드는 정점들의 셋이 기본적으로 반시계 방향으로 되어있는 지를 확인하는 것이 좋다. 만약 아니라도 해도 설정을 바꿈으로써 다르게도 할 수 있지만, 뒤죽박죽인 경우에는 묵념...

프리미티브의 winding order 은 **rasterization 단계**에서 계산된다. 이 단계는 3차원 삼각형 프리미티브를 픽셀로 변환하는 단계다. 이 단계에서 유저의 눈에 보이게 될 때, *face* 하고 있는 삼각형이 반시계 방향으로 그려진다고 하면, *back* 되고 있는 삼각형 프리미티브는 그려질 때 유저의 눈에서는 자연스레 시계 방향으로 그려지게 되는 것을 알 수 있다. 따라서 *back* 인 프리미티브는 자연스레 Culling 되어 Fragment Shader 호출이 되지 않는다.

![Image of viewer seeing front or back facing triangles](https://learnopengl.com/img/advanced/faceculling_frontback.png)

## Face Culling

![img](15_culling.png)



