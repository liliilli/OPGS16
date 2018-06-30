# DVector2

* [x] x, y 멤버 설정

* [x] 생성자 설정

  * [x] assimp `aiVector2D` 생성자 구현
  * [x] assimp `aiVector3D` 생성자 구현
    * 유저단에서는 안보이지만 내부에서는 `uv` 임포팅 등의 이유로 사용할 수 있게 해야함.
  * [x] glm `glm::vec2` 생성자 구현
  * [x] ~~bullet `btVector3` 생성자 구현 할 필요 없음.
  * [x] float `const float` 생성자 구현
  * [x] 일반 `float 2` 생성자 구현
  * [x] 일반 `DVector2` Copy CTOR 구현

* [x] 대입 연산자 구현

  * [x] assimp `aiVector2D` 대입 연산자 구현
  * [x] glm `glm::vec2` 대입 연산자 구현
  * [x] ~~bullet `btVector3` 대입 연산자 구현~~ 
    * 3 요소 벡터에서 2 요소 벡터로 좁게 변환하는 것은 말이 안된다.
  * [x] ~~일반 `const float` 대입 연산자 구현~~
    * 유저가 잘못해서 `float` 을 그대로 벡터에 넣어버리는 수가 있을 것 같다.

* [x] 일반 연산자 오버로딩 구현

  * [x] `operator+(DVector2)`
  * [x] `operator-(DVector2)`

  * [x] `operator*(float)`
  * [x] `operator*(DVector2)`
  * [x] `operator/(float)`
    * 만약 value 가 $$0$$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator/(DVector2)`
    * 만약 `DVector2` 의 값 중 하나라도 $$ 0 $$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator+=(DVector2)`
  * [x] `operator-=(DVector2)`
  * [x] `operator*=(float)`
  * [x] `operator*=(DVector2)`
  * [x] `operator/=(float)`
    * 만약 value 가 $$0$$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator/=(DVector2)`
    * 만약 `DVector2` 의 값 중 하나라도 $$ 0 $$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator<(DVector2, DVector2)`
    * DVector2 을 직접 비교하는 것은 `float` 특성상 부동소수점 오류로 오차가 있을 수 있기 때문에 `private` 로 해야함.
  * [x] `operator>(DVector2, DVector2)`
    * DVector2 을 직접 비교하는 것은 `float` 특성상 부동소수점 오류로 오차가 있을 수 있기 때문에 `private` 로 해야함.
  * [x] `operator<=(,)`
    * DVector2 을 직접 비교하는 것은 `float` 특성상 부동소수점 오류로 오차가 있을 수 있기 때문에 `private` 로 해야함.
  * [x] `operator>=(,)`
    * DVector2 을 직접 비교하는 것은 `float` 특성상 부동소수점 오류로 오차가 있을 수 있기 때문에 `private` 로 해야함.
  * [x] `operator==(,)`
    * 문제는 `float` 은 부동소수점 오류가 있기 때문에 이걸 바로 근사치로 잡아줘야 함. 위 `>=` 와 같은 연산자와는 다르게 근사적으로 같은가를 확인할 수 있으므로 `public` 으로 구현해도 됨.
    * `Helper/float.h` 에 `__ApproximateEqual` 로 근사적으로 같은가를 구현함.

* [x] 일반 멤버 함수 구현

  * [x] `GetLength()`
  * [x] `GetSquareLength()`
  * [x] `Normalize()`
  * [x] `Data()` :: 일차원 데이터 뭉치를 전달함.

* [ ] `Static` 함수 구현

  * [x] `Dot` 구현

  * [x] `Lerp` (Linear interpolation) 구현

    * $$ (x, y) $$ 와 $$ (z, w) $$ , 그리고 $$ a [0, 1] $$ 가 있다고 하면,
  $$
  \text{result} = (1 - a) (x, y) + (a) (z, w) 
  $$
  이다.

  * [x] ~~`Max`~~ 구현 (DVector2 와 DVector2 을 비교해서 무엇이 더 큰가를 반환)

    * `CompMaxLength` 로 바꿈. `Max` 는 의도를 전혀 파악하지 못 하고 있음.

  * [x] ~~`Min`~~ 구현 (DVector2 와 DVector2 을 비교해서 무엇이 더 작은가를 반환)

    * `CompoMinLength` 로 바꿈. `Min` 은 의도를 전혀 파악하지 못 하고 있음.

  * [ ] `Perpendicular` 구현 (DVector2 을 방향으로 보고, 이 방향에서 90도 반시계 방향을 반환)

  * [x] `AngleDeg` 구현 (DVector2 을 방향으로 보고, 두 DVector2 가 이루는 각을 Degree 로 반환함.

    * 3 번째 인자로 `normalized` ESwitch 인자를 둬서, normalized 가 `Off` 이면 두 인자를 노멀라이즈 하지 않고 바로 각도를 구하도록 한다. 하지만 ESwitch 를 유저단에서 쓰게 한다는 것은 좀 많이 빡신 것 같다...?

    * 애초에 구 벡터의 $$ \cos{\theta}$$ 을 구해서, 이걸 역코사인 변환 시키면 되지 않을까 하는 생각도 듬.

    * 평준화된 두 방향 벡터 $ \bar{a} = (x, y) $ 와 $$ \bar{b} = (x', y') $$ 가 있을 때, 이 둘에 대한 $ cos {\theta} $ 와 $$ \sin{\theta} $$ 는
  $$
  \cos{\theta} = xx + y'y \\
  \sin{\theta} = y'x - x'y
  $$
  이며, $$ \sin{\theta} $$ 가 양수인가 음수인가에 따라 $$ [0, 180) $$ 인가 $$ [-180, -0) $$ 인가를 알 수 있다.

  * [x] `AngleRad` 구현 (위와 같으나, Radian 으로 반환함)
  * [ ] 빌트인 객체 제공 구현
    * [ ] `Up` $$ (0.0, 1.0) $$
    * [ ] `Down` $$ (0.0, -1.0) $$
    * [ ] `Right` $$ (1.0, 0.0) $$
    * [ ] `Left` $$ (-1.0, 0.0) $$
    * `One` 이나 `Zero` 는 기본 생성자로 만드는 편이 간단하기 때문에 구현하지 않는다.

* [ ] 변환 연산자 구현

  * [ ] bullet 의 `dtVector3` 구현
  * [ ] `DVector3` 으로 구현