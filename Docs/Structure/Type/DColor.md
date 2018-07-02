# DColor

- [x] r, g, b, a `float` 멤버 설정.

  - 혹은 미리 `DClamp<type, from, to>` 을 만들어 버릴까? 지금은 그냥 `float` 으로만 처리하자.

- [x] 생성자 설정 (아직은 모르니까 기존에 만들었던 타입만 하는 것으로...)

  - [x] 일반 `float 3` 생성자 구현. 이 때 `a` 는 자동으로 $$ 1.0 $$ 이 되서 `Opacue` 처리가 됨.
  - [x] 일반 `float 4` 생성자 구현. 값들은 자동으로 Clamp 되어야함.

- [ ] 대입 연산자 구현

- [ ] 일반 연산자 오버로딩 구현

  - [ ] `operator==(,)`
    - 문제는 `float` 은 부동소수점 오류가 있기 때문에 이걸 바로 근사치로 잡아줘야 함. 위 `>=` 와 같은 연산자와는 다르게 근사적으로 같은가를 확인할 수 있으므로 `public` 으로 구현해도 됨.
    - `Helper/float.h` 에 `__ApproximateEqual` 로 근사적으로 같은가를 구현함.
  - 그 외의 연산자는 구현하지 않음. 왜냐하면 `DVector` 와는 다르게 `DColor` 은 컬러의 대소비교를 측정할 기준이 없기 때문임.
  - `+-/*` 연산자 역시 구현하지 않음. 각 요소를 조작하기 위해서는 그냥 `.r` `.g` `.b` `.a` 로 직접 접근해서 쓰면 됨.

- [ ] 일반 멤버 함수 구현

  - [x] `IsOpaque()` `a` 가 $$ 1.0 $$ 이면 opaque, 아니면 transparent 을 뱉는다.
  - [x] `SetOpaque()` `a` 을 $$ 1.0 $$ 으로 설정해서 opaque 함을 설정한다.
  - [x] `GetGrayScale()`  $$ (r, g, b) $$ 의 값을 그레이스케일화 시켜서 $$ [0, 1] $$ 사이의 값으로 반환한다.
    - ![{\displaystyle Y_{\mathrm {linear} }=0.2126R_{\mathrm {linear} }+0.7152G_{\mathrm {linear} }+0.0722B_{\mathrm {linear} }}](https://wikimedia.org/api/rest_v1/media/math/render/svg/f84d67895d0594a852efb4a5ac421bf45b7ed7a8) 

- [ ] `Static` 함수 구현

  - [ ] `Lerp` (Linear interpolation) 구현
    - $$ (x, y, z) $$ 와 $$ (z, w, v) $$ , 그리고 $$ a [0, 1] $$ 가 있다고 하면,

  $$
  \text{result} = (1 - a) (x, y) + (a) (z, w) 
  $$

  이다. 혹은 나중에 추가할 `opgs16::math::lerp` 에서 오버로딩 버전을 구현하면 될 것 같다만...

- [ ] 빌트인 객체 제공 구현

  - 여기서는 기본 색상에 대해서만 구현한다.

- [ ] 변환 연산자 구현
