# Chapter 13. Stencil Testing

각 프래그먼트들은 프래그먼트 쉐이더를 거친 이후에, **Stencil Testing** 을 실행하게 된다. 이 테스트는 이전의 *Depth testing* 과 같이 무수한 프래그먼트 중 일부만을 골라내는 테스트이다. 따라서 이 테스트를 통과한 나머지 프래그먼트가 Depth test 을 거치게 되고 결과적으로 Screen 에 보이게 된다. 

이 *Stencil Testing* 에서는 Depth test 에서 `z-value` 와 `Depth buffer` 을 썼듯이, **Stencil buffer** 을 써서 테스트를 하게 된다. 이 테스트로 여러가지 이펙트를 줄 수 있다.

* *Stencil Buffer* 는 대개 픽셀 혹은 프래그먼트 당 *8 bit*의 공간을 가지는 각각의 **Stencil Value** 을 가진다. 이 값은 0 에서 255 까지의 값을 저장할 수 있다. 
  이 값들을 프로그래머가 임의로 설정이 가능하기 때문에, 이 값을 설정해 Test 에서 조건에 맞을 경우 프래그먼트를 살릴지 죽일지 결정이 가능하다.

> 사실 이 Stencil buffer 는 거의 모든 GUI 윈도우 생성 라이브러리에서 필요한 존재다. GLFW 는 자동으로 스텐실 버퍼를 만들어 주기 때문에 따로 생성할 필요는 없으나, 다른 OpenGL 라이브러리 등과 같은 경우라면 스텐실 버퍼를 생성해야 할지 말지 메뉴얼을 통해 자세히 알아보는 것이 좋다.

스텐실 버퍼의 간단한 예제는 다음과 같다.

![A simple demonstration of a stencil buffer](https://learnopengl.com/img/advanced/stencil_buffer.png)

위의 이미지를 예시로 들어 테스트를 설명하면, 우선 Clear 된 Stencil Buffer ( 이 역시 매 프레임마다 Clear 해야한다 ) 는 모든 픽셀 혹은 모든 프래그먼트에 대해 $$ 0 $$ 의 값을 가진다. 그리고 프로그래머가 임의로 사각형의 $$ 1 $$ 값을 픽셀 어딘가에 집어넣는다. 그러면 테스트 도중에 스텐실 버퍼에서 $$ 1 $$ 값을 가진 부분만 렌더링이 되서 Depth testing 에 들어가게 된다.

*스텐실 테스팅* 에서는 프래그먼트들을 렌더링 할 때마다 버퍼에 구체적인 값을 넣을 수 있도록 한다. 따라서 자기가 어떻게 하던지 여러가지 효과를 구사할 수 있다. 

그러나 **대략적인 틀**은 존재한다.

1. 스텐실 버퍼에 대해서 쓰기 권한을 활성화 시킨다.
2. 오브젝트를 렌더링하고, 스텐실 버퍼의 값들을 갱신시킨다.
3. 스텐실 버퍼의 쓰기 권한을 비활성화한다.
4. 스텐실 테스트의 대상이 된 오브젝트를 제외한 다른 오브젝트를 렌더링한다.

 ## Implementation

스텐실 테스트를 할려면 `glEnable` 과 `GL_STENCIL_TEST` 인자로 활성화를 시켜야 한다.

``` c++
glEnable(GL_STENCIL_TEST);
```

또한 이전 테스트와 마찬가지로 스텐실 버퍼 역시 매 프레임마다 초기화 시킨다.

``` c++
glClear(... | GL_STENCIL_BUFFER_BIT);
```

`glDepthMask()` 처럼 스텐실 테스트 역시 `0xFF` 와 `0x00` 을 `glStencilMask()` 함수에 넣어 버퍼를 읽기 전용으로만 할지 말지를 결정할 수 있다. 기본 OpenGL 은 `0xFF` 값으로 설정되어 있다.

### Stencil Functions

OpenGL 은 Stencil Testing 의 동작을 설정하는데 필요한 몇 가지 메커니즘을 제공한다. 하나는 `glStencilFunc` 이며 또 하나는 `glStencilOp` 이다.

`glStencilFunc(GLenum func, GLint ref, GLuint mask)` 는 다음 3 가지 파라미터를 가진다.

* `func` : 미리 정해진 Stencil Function 을 설정한다. 이 함수의 값이 `glStencilFunc` 의 `ref` 값과 스텐실 값에 저장된다.
  가능한 옵션으로는 GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL and GL_ALWAYS 가 존재하며 의미는 Depth testing 때와 거의 같다.
* `ref` : 스텐실 테스트에서 사용할 값을 설정한다. 스텐실 테스트에서 참조하는 모든 값이 이 값과 비교된다.
* `mask` : ref value 와 stencil value 에 AND 연산을 하는 8 bit 값을 말한다. 마스킹 후에 스텐실 테스트 연산이 이루어진다. 기본 0x11111111 로 세팅되어 있다.

그런데 이 함수는 **단지** OpenGL 에게 무엇을 해야할지만 알려줄 뿐이지, 어떻게 스텐실 버퍼를 **업데이트**하는지는 가르쳐주지 않는다. 이 때 쓰는 함수가 `glStencilOp()` 함수이다.

`glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)` 함수는 다음 3 가지 파라미터를 가진다.

* `sfail` : 스텐실 테스트가 실패시에 할 행동
* `dpfail` : 스텐실 테스트는 통과했지만, 깊이 테스트가 실패할 경우의 행동
* `dppass` : 스텐실 테스트와 깊이 테스트 모두 통과했을 때의 행동

각 옵션들은 다음 중에서 하나를 골라 쓸 수 있다.

| Action         | Description                              |
| -------------- | ---------------------------------------- |
| `GL_KEEP`      | The currently stored stencil value is kept. |
| `GL_ZERO`      | The stencil value is set to `0`.         |
| `GL_REPLACE`   | The stencil value is replaced with the reference value set with glStencilFunc. |
| `GL_INCR`      | The stencil value is increased by `1` if it is lower than the maximum value. |
| `GL_INCR_WRAP` | Same as GL_INCR, but wraps it back to `0` as soon as the maximum value is exceeded. |
| `GL_DECR`      | The stencil value is decreased by `1` if it is higher than the minimum value. |
| `GL_DECR_WRAP` | Same as GL_DECR, but wraps it to the maximum value if it ends up lower than `0`. |
| `GL_INVERT`    | Bitwise inverts the current stencil buffer value. |

기본 `glStencilOp` 함수의 인자는 (GL_KEEP, GL_KEEP, GL_KEEP) 으로 설정되어 있다. 이는 스텐실이 실패하든지 뭐가 실패하든지 간에 스텐실 버퍼의 값은 변하지 않는다는 것을 의미한다. 모든 테스트를 통과했을 때 마저 GL_KEEP 이기 때문에 아무것도 변하지 않는다.

따라서 스텐실 테스팅을 통해 무언가를 효과를 내고 싶을 때는 이 함수를 통해 인자를 커스터마이징 해야한다.

### Object outlining

스텐실 테스트를 이용해서 outlining 을 할 수 있도록 해보자.

![An object outlined using stencil testing/buffer](https://learnopengl.com/img/advanced/stencil_object_outlining.png)

3D 오브젝트의 외곽선을 따와서 표시할려면 우선 오브젝트를 감싸는 단일 색의 오브젝트가 필요하다. 이 기법은 전략 게임이나 아니면 어떤 오브젝트를 선택하고자 할 때 매우 유용한 이펙트가 될 수 있다. 다음은 이 기법을 구현하는 순서이다.

1. 외곽선을 그릴 오브젝트를 렌더링 하기 이전에, 스텐실 함수의 인자를 **GL_ALWAYS** , 그리고 외곽선을 그릴 오브젝트의 프래그먼트에 따라 값이 변경될 수 있도록 치환 값을 $$ 1 $$ 로 둔다.
2. 대상 오브젝트를 렌더링한다.
3. 스텐실 마스킹을 멈추고, 깊이 테스트 또한 멈춘다.
4. 오브젝트를 살짝 스케일을 늘린다.
5. 단일 색을 출력하는 다른 쉐이더 프로그램을 구동시킨다.
6. 오브젝트를 다시 그린다. 하지만 이 때는 스텐실 버퍼가 $$ 1 $$ 이 아닌 경우에 대해서만 그리도록 한다. (즉, 오브젝트가 그려진 부분을 제외한다)
7. 스텐실 마스킹과 깊이 테스트를 재활성화한다.


그러면 다음과 같이 코드가 생성될 것이다.

``` c++
// Procedure 1 ~ 2 (이 상황에서는 STENCIL_BUFFER 의 값들이 모두 0이다)
glStencilFunc(GL_ALWAYS, 1, 0xFF);
glStencilMask(0xFF);

// 그리고 싶은 오브젝트를 그린다.

// Procedure 3 ~ 6
glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilMask(0x00);
glDisable(GL_DEPTH_TEST);

// 외곽선 효과를 내고 싶은 오브젝트에 적용할 model_matrix 을 스케일링한다.
// 그리고 오브젝트를 다시 그린다.

glStencilMask(0xFF);
glEnable(GL_DEPTH_TEST);
```

이 때 조심해야 할 점은, 모델 메시의 모양에 따라서 완벽하게 길이가 일정한 외곽선이 그려지지 않는다는 점과, Depth test 을 거치지 않기 때문에 그려진 순서대로의 묘화 우선순위를 가진다는 점이다.

결과는 다음과 같다.

![img](12_stencil.png)



